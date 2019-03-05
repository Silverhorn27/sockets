#ifndef SERVER_H
#define SERVER_H

#include "definions.h"
#include <iostream>
#include <exception>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class BindServerExeption : public std::exception {
public:
    virtual const char* what() const noexcept {
        return "Error binding server";
    }
};

class Server {
private:
    int _fd;
    struct sockaddr_in _fdaddr;
    std::string ip;
    int port;
public:
    Server() = default;
    Server(const string &ip, int port);
    void init(const string &ip, int port);
    void start();
};

#endif //SERVER_H