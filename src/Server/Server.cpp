#include "Server.h"

#include <algorithm>
#include <set>
#include <thread>
#include <poll.h>
#include <cstring>
#include "ConnectionHandler.h"

static const string SERVER_IP = "server.ip";
static const string SERVER_PORT = "server.port";

Server::Server(const ConfigReader &config)
    : _fd(-1)
    , _fdaddr()
    , ip(config[SERVER_IP])
    , port(atoi(config[SERVER_PORT].c_str()))
    , _requestStop(false)
    , _threadPool(8)
    , _logger(StringUtils::toString("Server ", StringUtils::hexToString(this)))
{
    init(ip, port);
}

void Server::setFactory(std::unique_ptr<InteractorInterfaceFactory> factory)
{
    _factory = std::move(factory);
}

void Server::init(const string &ip, int port)
{
    _requestStop = false;
    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    std::set<int> slave_sockets;
    _fdaddr.sin_family = AF_INET;
    _fdaddr.sin_port = htons(static_cast<uint16_t>(port));
    _fdaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (inet_pton(AF_INET, ip.c_str(), &_fdaddr.sin_addr) == -1) {
        _logger.log(Logger::Error, "Error in inet_pton()");
    }
}

/*
void Server::start() {
    int enable = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        std::cout << "setsockopt(SO_REUSEADDR) failed" << std::endl;
    }
    if (bind(_fd, (struct sockaddr *)(&_fdaddr), sizeof(_fdaddr)) == -1) {
        throw BindServerExeption();
    }
    listen(_fd, SOMAXCONN);
    std::set<int> slave_sockets;

    while(true) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(_fd, &set);
        for(auto &socket : slave_sockets) {
            FD_SET(socket, &set);
        }

        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        int max = std::max(_fd, *std::max_element(slave_sockets.begin(), slave_sockets.end()));
        select(max+1, &set, NULL, NULL, NULL);
        for(auto &socket : slave_sockets) {
            if (FD_ISSET(socket, &set)) {
                if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                    std::cout << ("setsockopt faile\n");
                }
                if (setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                    std::cout << ("setsockopt faile\n");
                }
                static char buffer[1024];
                int recv_size = recv(socket, buffer, 1024, MSG_NOSIGNAL);
                if (recv_size <= 0) {
                    if (recv_size < 0) {
			            std::cout << "error. recv_size is: " << recv_size << std::endl;
		            }
                    close(socket);
                    slave_sockets.erase(socket);
                } else {
                    send(socket, buffer, recv_size, MSG_NOSIGNAL);
                }
            }
        }
        if (FD_ISSET(_fd, &set)) {
            int slave_socket = accept(_fd, 0, 0);
            slave_sockets.insert(slave_socket);
        }
    }
}
*/

void Server::startClientAcceptor()
{
    int enable = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        _logger.log(Logger::Error, "setsockopt(SO_REUSEADDR) failed");
    }
    if (bind(_fd, reinterpret_cast<struct sockaddr *>(&_fdaddr), sizeof(_fdaddr)) < 0) {
        throw BindServerExeption();
    }
    listen(_fd, SOMAXCONN);

    const int timeoutInMsec = 10000;
    while (!_requestStop) {
        struct pollfd pollData{};
        bzero(&pollData, sizeof(pollData));
        pollData.fd = _fd;
        pollData.events = POLLIN;
        int ret = poll(&pollData, 1, timeoutInMsec);
        if (ret > 0) {
            int slavefd = accept(pollData.fd, nullptr, nullptr);
            ConnectionHandler::Ptr newConnection(std::make_unique<ConnectionHandler>(_factory->createInteractorObject(), slavefd));
            _threadPool.start(std::move(newConnection), true);

        } else {
            _threadPool.stopAllThreads();
            close(_fd);
            _requestStop = true;
        }
    }
}
