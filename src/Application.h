#ifndef APPLICATION_H
#define APPLICATION_H

#include "definions.h"
#include "Logger/Logger.h"
#include "Server/Server.h"
#include "Utils/ConfigReader.h"
#include <memory>
#include <vector>
using std::unique_ptr;

class Application
{
    Logger _logger;
    unique_ptr<Server> _server;
    vector<string> _args;
    ConfigReader _config;

public:
    Application(int argc, char **argv);

    const vector<string>& getArgs() const;

    int start();

    void printArgs();

    void waitRequestForStop();

private:
    void init(int argc, char **argv);
};

#endif // APPLICATION_H
