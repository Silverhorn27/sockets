#ifndef APP_CLIENT_H
#define APP_CLIENT_H

#include <iostream>
#include <memory>

#include "definions.h"
#include "Client/Client.h"
#include "Logger/Logger.h"
#include "Application/Application.h"

using std::unique_ptr;

class AppClient : public Application  {
    Logger _logger;
    unique_ptr<Client> _client;
    ConfigReader _config;

public:
    AppClient(int argc, char **argv);

    int start() override;

};

#endif //APP_CLIENT_H
