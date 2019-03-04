#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Server {
private:
    int _fd;
    struct sockaddr_in _fdaddr;
    std::string ip;
    int port;
public:
    Server(const std::string &ip, int port);
    void start();
};

#endif //SERVER_H