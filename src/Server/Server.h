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
#include "ConnectionHandler.h"
#include "Utils/ConfigReader.h"
#include <mutex>
#include <set>


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
    string ip;
    int port;
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
    void init(const string &ip, int port);
    void start();
    void requestStop();
    void waitFinished();

public:
    void run() override;
};

#endif //SERVER_H
