#include "Client.h"

static const string SERVER_IP = "server.ip";
static const string SERVER_PORT = "server.port";

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
    char buffer[] = "exit\n";
    connect(_fd, reinterpret_cast<struct sockaddr *>(&_fdaddr), sizeof(_fdaddr));
    send(_fd, buffer, sizeof(buffer), MSG_NOSIGNAL);
}
