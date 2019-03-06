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
#include "Mutlithreading/All.h"
#include "Logger/Logger.h"
#include "InteractorInterfaceFactory.h"


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
    bool _requestStop;
    ThreadPool _threadPool;
    Logger _logger;
    InteractorInterfaceFactory *_factory;
    
public:
    Server() = default;
    Server(const string &ip, int port);
    void setFactory(InteractorInterfaceFactory *factory);
    void init(const string &ip, int port);
    void start();
    void startClientAcceptor();
    void requestStop();
};

#endif //SERVER_H
