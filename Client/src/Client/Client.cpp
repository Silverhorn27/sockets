#include "Client.h"
#include "ConnectionHandler.h"

#include <cstring>

static const string SERVER_IP = "server.ip";
static const string SERVER_PORT = "server.port";

Client::~Client()
{
    _workerThread.stopLoop();
    while (_workerThread.isUsed()) {
        Thread::sleep(10);
    }

    requestStop();
}

Client::Client(const ConfigReader &config)
    : _logger(StringUtils::toString("Server ", StringUtils::hexToString(this)))
    , _fd(-1)
    , _fdaddr()
    , _ip(config[SERVER_IP])
    , _port(atoi(config[SERVER_PORT].c_str()))
    , _requestStop(false)
{
    init(_ip, _port);
}

void Client::init(const string &ip, int port)
{
    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    _fdaddr.sin_family = AF_INET;
    _fdaddr.sin_port = htons(static_cast<uint16_t>(port));
    if (inet_pton(AF_INET, ip.c_str(), &_fdaddr.sin_addr) == -1) {
        _logger.log(Logger::Error, "Error in inet_pton()");
    }
}

void Client::start()
{
    _workerThread.setRunnable(this, false);
    Thread::sleep(300);
    for (int i = 0; i < 1000; ++i){
        string str = string("hello ") + StringUtils::toString(i);
        dynamic_cast<ConnectionHandler *>(_handler.get())->sendData(str.data(), str.length());
    }
}

void Client::requestStop()
{
    close(_fd);
    _workerThread.stopLoop();
    _requestStop = true;
}

void Client::run()
{
    TRACE_FUNCTION();
    std::unique_lock runningLock(_runningMutex);
    if (-1 == connect(_fd, reinterpret_cast<struct sockaddr *>(&_fdaddr), sizeof (_fdaddr))) {
        _logger.log(Logger::Error, "Failed to connect to server");
    }

    _handler.reset(new ConnectionHandler(std::unique_ptr<InteractorInterface>(new FilesystemInterator),
                              _fd, [](class ConnectionHandler *){}));

    _handler->run();
}
