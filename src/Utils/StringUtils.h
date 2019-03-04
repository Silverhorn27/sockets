#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "definions.h"
#include <sstream>

class StringUtils
{
    static string toString()
    {
        return string();
    }

public:
    template <typename T>
    static string toString(const T &value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    template <typename T, typename ...Args>
    static string toString(const T &value, const Args&... args)
    {
        return toString(value) + toString(args...);
    }

    template <typename T>
    static string hexToString(const T &value)
    {
        std::stringstream ss;
        ss << std::hex << value;
        return string("0x") + ss.str();
    }
};

#endif // STRING_UTILS_H
