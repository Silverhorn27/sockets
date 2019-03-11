#include "Logger.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>
#include <algorithm>

using std::chrono::system_clock;
using std::chrono::duration_cast;

std::mutex Logger::_mutex;
std::atomic<Logger::Level> Logger::_logLevel(Logger::Level::Info);

Logger::Logger(const string &name)
    : _name(name)
{

}

void Logger::setLevel(Logger::Level level)
{
    Logger::_logLevel = level;
}

char Logger::describeLevel(Logger::Level level)
{
    switch (level)
    {
    case Trace:   return 'T';
    case Debug:   return 'D';
    case Info:    return 'I';
    case Notice:  return 'N';
    case Warning: return 'W';
    case Error:   return 'E';
    case Critical:return 'C';
    default:;
    }
    return 'U'; // Unknown
}

Logger::Level Logger::levelFromName(string name)
{
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    if (name == "trace") {
        return Level::Trace;
    }
    if (name == "debug") {
        return Level::Debug;
    }
    if (name == "info") {
        return Level::Info;
    }
    if (name == "notice") {
        return Level::Notice;
    }
    if (name == "warning") {
        return Level::Warning;
    }
    if (name == "error") {
        return Level::Error;
    }
    if (name == "critical") {
        return Level::Critical;
    }
    return Level::Invalid;
}

static string timeToString(int value, size_t len = 2)
{
    string ret;
    string str = StringUtils::toString(value);
    size_t it = len - str.length();
    for (size_t i = 0; i < it; ++i) {
        ret += "0";
    }
    return ret + str;
}

void Logger::logImpl(const std::string &line)
{
    auto timepoint = system_clock::now();
    auto hours = timeToString(duration_cast<std::chrono::hours>(timepoint.time_since_epoch()).count() % 24);
    auto minutes = timeToString(duration_cast<std::chrono::minutes>(timepoint.time_since_epoch()).count() % 60);
    auto seconds = timeToString(duration_cast<std::chrono::seconds>(timepoint.time_since_epoch()).count() % 60);
    auto microseconds = timeToString(duration_cast<std::chrono::microseconds>(timepoint.time_since_epoch()).count() % (1000*1000), 6);

    std::unique_lock locker(Logger::_mutex);
    std::cout << hours << ":" << minutes << ":" << seconds << "." << microseconds
              << " [" << (std::this_thread::get_id()) << "]: " << line << std::endl;
}
