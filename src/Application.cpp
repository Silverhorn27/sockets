#include "Application.h"
#include "Filesystem/FilesystemInteractorFactory.h"

static const string LOGGER_LEVEL = "logger.level";

Application::Application(int argc, char **argv)
    : _logger(StringUtils::toString("Application ", StringUtils::hexToString(this)))
    , _server(nullptr)
    , _config(CONFIG_FILE)
{
    try {
        init(argc, argv);
    } catch (std::exception &ex) {
        _logger.log(Logger::Critical, "Caught '", ex.what(), "'. At ", __FILE__, ':', __LINE__);
        throw;
    }
}

void Application::init(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i) {
        _args.emplace_back(argv[i]);
    }

    Logger::setLevel(Logger::levelFromName(_config[LOGGER_LEVEL]));
}

const vector<string>& Application::getArgs() const
{
    return _args;
}

int Application::start()
{
    try {
        printArgs();

        _logger.log(Logger::Notice, "Server starting...");

        _server.reset(new Server(_config));

        _server->setFactory(std::unique_ptr<FilesystemInteractorFactory>(new FilesystemInteractorFactory));

        _server->startClientAcceptor();

        return 0;
    } catch (std::exception &ex) {
        _logger.log(Logger::Critical, "Caught '", ex.what(), "'. At ", __FILE__, ':', __LINE__);
    }

    return 1;
}

void Application::printArgs()
{
    string msg = "\n";
    for (const auto &arg : getArgs()) {
        msg += (arg + "\n");
    }

    _logger.log(Logger::Info, "Running with args:", msg);
}
