#ifndef APPLICATION_H
#define APPLICATION_H

#include "definions.h"
#include "Logger/Logger.h"
#include "Server/Server.h"
#include <vector>
using std::vector;

class Application
{
    Logger _logger;
    Server _server;
    vector<string> _args;

public:
    Application(int argc, char **argv);

    const vector<string>& getArgs() const;

    int start();

    void printArgs();

private:
    void init(int argc, char **argv);
};

#endif // APPLICATION_H
