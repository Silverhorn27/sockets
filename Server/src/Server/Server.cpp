#include "Server.h"

#include <thread>
#include <poll.h>
#include <cstring>
#include "ConnectionHandler.h"

static const string SERVER_IP = "server.ip";
static const string SERVER_PORT = "server.port";

Server::~Server()
{
    _workerThread.stopLoop();
    while (_workerThread.isUsed()) {
        Thread::sleep(10);
    }

    requestStop();
    waitFinished();
}

Server::Server(const ConfigReader &config)
    : _fd(-1)
    , _fdaddr()
    , _ip(config[SERVER_IP])
    , _port(atoi(config[SERVER_PORT].c_str()))
    , _requestStop(false)
    , _threadPool(8)
    , _logger(StringUtils::toString("Server ", StringUtils::hexToString(this)))
{
    init(_ip, _port);
}

void Server::setFactory(std::unique_ptr<InteractorInterfaceFactory> factory)
{
    _factory = std::move(factory);
}

void Server::init(const string &ip, int port)
{
    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    _fdaddr.sin_family = AF_INET;
    _fdaddr.sin_port = htons(static_cast<uint16_t>(port));
    _fdaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (inet_pton(AF_INET, ip.c_str(), &_fdaddr.sin_addr) == -1) {
        _logger.log(Logger::Error, "Error in inet_pton()");
    }
}

void Server::start()
{
    _workerThread.setRunnable(this, false);
}

void Server::requestStop()
{
    close(_fd);
    _workerThread.stopLoop();
    _threadPool.stopAllThreads();
    _requestStop = true;

    std::unique_lock locker(_handlersMutex);
    for (ConnectionHandler *handler : _handlers) {
        handler->requestStop();
    }
}

void Server::waitFinished()
{
    std::unique_lock runningLock(_runningMutex);
    _threadPool.waitAll();
}

void Server::run()
{
    std::unique_lock runningLock(_runningMutex);

    int enable = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        _logger.log(Logger::Error, "setsockopt(SO_REUSEADDR) failed");
    }
    if (bind(_fd, reinterpret_cast<struct sockaddr *>(&_fdaddr), sizeof(_fdaddr)) < 0) {
        throw BindServerExeption();
    }
    listen(_fd, SOMAXCONN);

    const int timeoutInMsec = 1000;

    auto deleteCallback = [this] (ConnectionHandler *handlerCaller) {
        std::unique_lock locker(_handlersMutex);
        _handlers.erase(handlerCaller);
    };

    while (!_requestStop) {
        struct pollfd pollData{};
        bzero(&pollData, sizeof(pollData));
        pollData.fd = _fd;
        pollData.events = POLLIN;

        int ret = poll(&pollData, 1, timeoutInMsec);

        if (ret > 0 && pollData.revents & POLLIN) {
            int slavefd = accept(pollData.fd, nullptr, nullptr);
            auto newConnection = std::make_unique<ConnectionHandler>(
                        _factory->createInteractorObject(), slavefd, deleteCallback);
            {
                std::unique_lock locker(_handlersMutex);
                _handlers.insert(newConnection.get());
            }
            _threadPool.start(std::move(newConnection), true);

        } else if (ret != 0) {
            requestStop();
        } else {
            _logger.log(Logger::Debug, "Server timeout");
        }
    }
}
