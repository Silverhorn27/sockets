#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include "definions.h"

using Config = map<string, string>;

class ConfigReader
{
    Config _config;

public:
    ConfigReader(const string &configFile);

    const string &operator[] (const string &key) const;

private:
    void init(const string &configFile);
    void initFromString(const string &config);
};

class ConfigReaderException : public std::exception
{
    string _msg;

public:
    ConfigReaderException(const string &msg);
    const char *what() const noexcept override;
};

class ConfigNotFoundException : public ConfigReaderException
{
public:
    ConfigNotFoundException(const string &msg);
};

#endif // CONFIG_READER_H
