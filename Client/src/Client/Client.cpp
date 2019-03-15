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
}

void Client::requestStop()
{
    close(_fd);
    _workerThread.stopLoop();
    _requestStop = true;
}

void Client::run()
{
    _logger.log(Logger::Debug, "run()");
    std::unique_lock runningLock(_runningMutex);
    std::cout << "dasfadsf";
    connect(_fd, reinterpret_cast<struct sockaddr *>(&_fdaddr), sizeof (_fdaddr));
    char Buffer[] = "close";
        send(_fd, Buffer, sizeof(Buffer), MSG_NOSIGNAL);
        recv(_fd, Buffer, sizeof(Buffer), MSG_NOSIGNAL);
}
