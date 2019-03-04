#include "Logger.h"
#include <iostream>
#include <chrono>
#include <thread>

using std::chrono::system_clock;
using std::chrono::duration_cast;

std::mutex Logger::_mutex;

Logger::Logger(const std::__cxx11::string &name)
    : _name(name)
{

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
