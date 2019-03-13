#ifndef APP_SERVER_H
#define APP_SERVER_H

#include <iostream>
#include <memory>

#include "definions.h"
#include "Server/Server.h"
#include "Logger/Logger.h"
#include "Utils/ConfigReader.h"
#include "Application/Application.h"

using std::unique_ptr;

class AppServer : public Application {
    Logger _logger;
    unique_ptr<Server> _server;
    ConfigReader _config;

public:
    AppServer(int argc, char **argv);

    int start() override;

    void waitRequestForStop();

private:
    void init();
};

#endif //APP_SERVER_H
