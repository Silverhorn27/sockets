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

class Client {
    Logger _logger;
    int _fd;
    struct sockaddr_in _fdaddr;
    string _ip;
    int _port;
    bool _requestStop;

public:
    Client(const ConfigReader &config);

    void start();

private:
    void init(const string &ip, int port);
};

#endif //CLIENT_H
