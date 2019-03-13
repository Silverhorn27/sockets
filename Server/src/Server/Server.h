#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <exception>
#include <mutex>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "definions.h"
#include "Logger/Logger.h"
#include "Utils/ConfigReader.h"
#include "Mutlithreading/All.h"
#include "Server/ConnectionHandler.h"
#include "Factory/InteractorInterfaceFactory.h"

class BindServerExeption : public std::exception {
public:
    virtual const char* what() const noexcept {
        return "Error binding server";
    }
};

class Server : public Runnable {
private:
    int _fd;
    struct sockaddr_in _fdaddr;
    string _ip;
    int _port;
    bool _requestStop;
    ThreadPool _threadPool;
    Logger _logger;
    std::unique_ptr<InteractorInterfaceFactory> _factory;
    std::mutex _runningMutex;
    Thread _workerThread;
    std::mutex _handlersMutex;
    std::set<ConnectionHandler *> _handlers;
    
public:
    ~Server() override;
    Server(const ConfigReader &config);
    void setFactory(std::unique_ptr<InteractorInterfaceFactory> factory);
    void start();
    void requestStop();
    void waitFinished();

public:
    void run() override;
    void init(const string &ip, int port);
};

#endif //SERVER_H
