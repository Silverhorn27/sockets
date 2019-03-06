#include "Application.h"
#include "Filesystem/FilesystemInteractorFactory.h"

Application::Application(int argc, char **argv)
    : _logger(StringUtils::toString("Application ", StringUtils::hexToString(this))),
    _server("127.0.0.1", 4444)
{
    _server.setFactory(new FilesystemInteractorFactory);

    init(argc, argv);
    if (_args.size() == 3) {
        _logger.log(Logger::Notice, "Create server");
    } else {
        _logger.log(Logger::Error, "Was passed wrong arguments");
    }
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

    _logger.log(Logger::Notice, "Server starting...");
    
    _server.startClientAcceptor();

    return 0;
}

void Application::printArgs()
{
    string msg = "\n";
    for (const auto &arg : getArgs()) {
        msg += (arg + "\n");
    }

    _logger.log(Logger::Info, "Running with args:", msg);
}
