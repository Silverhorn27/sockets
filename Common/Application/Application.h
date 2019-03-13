#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <vector>
#include "definions.h"
#include "Logger/Logger.h"

class Application
{
    Logger _logger;
    vector<string> _args;

public:
    Application(int argc, char **argv);

    const vector<string>& getArgs() const;

    void printArgs();

    virtual int start() = 0;

    virtual ~Application() = default;

private:
    void init(int argc, char **argv);
};

#endif // APPLICATION_H
