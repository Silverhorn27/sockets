#ifndef LOGGER_H
#define LOGGER_H

#include "definions.h"
#include "Utils/StringUtils.h"
#include <mutex>
#include <atomic>

class Logger
{
public:
    enum Level : int {
        Trace,
        Debug,
        Info,
        Notice,
        Warning,
        Error,
        Critical
    };

private:
    string _name;
    static std::mutex _mutex;
    static std::atomic<Level> _logLevel;

public:

    Logger(const string &name);

    template <typename ...Args>
    void log(Level logLevel, const Args&... args)
    {
        if (_logLevel <= logLevel) {
            logImpl(StringUtils::toString('[', describeLevel(logLevel), "] ", _name, ": ", args...));
        }
    }

    static void setLevel(Level level);
    static char describeLevel(Level level);

private:
    static void logImpl(const string &line);

};

#endif // LOGGER_H
