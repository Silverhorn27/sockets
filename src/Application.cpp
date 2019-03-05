#include "Application.h"

Application::Application(int argc, char **argv)
    : _logger(StringUtils::toString("Application ", StringUtils::hexToString(this)))
{
    init(argc, argv);
    _server.init(argv[1], atoi(argv[2]));
}

void Application::init(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i) {
        _args.emplace_back(argv[i]);
    }
}

const vector<string>& Application::getArgs() const
{
    return _args;
}

int Application::start()
{
    printArgs();
    _logger.log("Server starting...");
    
    _server.start();
    // do something, start server ...

    return 0;
}

void Application::printArgs()
{
    string msg = "\n";
    for (const auto &arg : getArgs()) {
        msg += (arg + "\n");
    }

    _logger.log("Running with args:", msg);
}
