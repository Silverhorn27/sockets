#include "Application.h"

Application::Application(int argc, char **argv)
    : _logger(StringUtils::toString("Application ", StringUtils::hexToString(this)))
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
}

const vector<string>& Application::getArgs() const
{
    return _args;
}

void Application::printArgs()
{
    string msg = "\n";
    for (const auto &arg : getArgs()) {
        msg += (arg + "\n");
    }

    _logger.log(Logger::Info, "Running with args:", msg);
}


