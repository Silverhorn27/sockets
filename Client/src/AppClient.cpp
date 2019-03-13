#include "AppClient.h"

AppClient::AppClient(int argc, char **argv)
    : Application (argc, argv)
    , _logger(StringUtils::toString("Application ", StringUtils::hexToString(this)))
    , _client(nullptr)
    , _config(CONFIG_FILE)
{

}

int AppClient::start()
{
    _logger.log(Logger::Notice, "Client starting...");

    _client = std::make_unique<Client>(_config);

    _client->start();

    return 0;
}
