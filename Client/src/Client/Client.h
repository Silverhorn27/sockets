#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <exception>
#include <memory>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "definions.h"
#include "Logger/Logger.h"
#include "Utils/ConfigReader.h"
#include "Mutlithreading/All.h"
#include "Factory/InteractorInterfaceFactory.h"
#include "Filesystem/FilesystemInteractor.h"
#include "ConnectionHandler.h"

class Client : public Runnable {
    Logger _logger;
    int _fd;
    struct sockaddr_in _fdaddr;
    string _ip;
    int _port;
    bool _requestStop;
    Thread _workerThread;
    std::mutex _runningMutex;
    ConnectionHandler::Ptr _handler;

public:
    ~Client() override;
    Client(const ConfigReader &config);
    void start();
    void requestStop();

public:
    void run() override;
    void init(const string &ip, int port);
};

#endif //CLIENT_H
