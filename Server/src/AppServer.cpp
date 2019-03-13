#include "AppServer.h"
#include "Filesystem/FilesystemInteractorFactory.h"

static const string LOGGER_LEVEL = "logger.level";

AppServer::AppServer(int argc, char **argv)
    : Application (argc, argv)
    , _logger(StringUtils::toString("Application ", StringUtils::hexToString(this)))
    , _config(CONFIG_FILE)
{
    try {
        init();
    } catch (std::exception &ex) {
        _logger.log(Logger::Critical, "Caught '", ex.what(), "'. At ", __FILE__, ':', __LINE__);
        throw;
    }
}

int AppServer::start() {
    try {

        _logger.log(Logger::Notice, "Server starting...");

        _server = std::make_unique<Server>(_config);

        _server->setFactory(std::make_unique<FilesystemInteractorFactory>());

        _server->start();

        waitRequestForStop();

        _server->requestStop();
        _server->waitFinished();

        return 0;
    } catch (std::exception &ex) {
        _logger.log(Logger::Critical, "Caught '", ex.what(), "'. At ", __FILE__, ':', __LINE__);
    }

    return 1;
}

void AppServer::init()
{
    Logger::setLevel(Logger::levelFromName(_config[LOGGER_LEVEL]));
}

void AppServer::waitRequestForStop()
{
    string line;
    do {
        std::cin >> line;
    } while (line != "exit");
}
