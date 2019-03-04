#ifndef LOGGER_H
#define LOGGER_H

#include "definions.h"
#include "Utils/StringUtils.h"
#include <mutex>

class Logger
{
    string _name;
    static std::mutex _mutex;

public:
    Logger(const string &name);

    template <typename ...Args>
    void log(const Args&... args)
    {
        logImpl(StringUtils::toString(_name, ": ", args...));
    }

private:
    static void logImpl(const string &line);

};

#endif // LOGGER_H
