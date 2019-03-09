#include "ConfigReader.h"
#include "StringUtils.h"
#include <fstream>
#include <sys/stat.h>
using std::ifstream;

ConfigReader::ConfigReader(const std::string &configFile)
{
    init(configFile);
}

const string &ConfigReader::operator[](const string &key) const
{
    auto it = _config.find(key);
    if (it == _config.end()) {
        throw ConfigNotFoundException(StringUtils::toString("Unable to find key '", key, "'. At ", __FILE__, ':', __LINE__));
    }

    return it->second;
}

static size_t getFileSize(const string &filename)
{
    struct stat filestat;
    if (stat(filename.c_str(), &filestat) != 0) {
        throw ConfigReaderException(StringUtils::toString("Failed to get filesize " +
                                                          filename + ". At " + __FILE__ + ":", __LINE__));
    }

    return static_cast<size_t>(filestat.st_size);
}

void ConfigReader::init(const std::string &configFile)
{
    ifstream file(configFile);
    if (!file.is_open()) {
        throw ConfigReaderException(StringUtils::toString("Failed to open file " +
                                                          configFile + ". At " + __FILE__ + ":", __LINE__));
    }

    auto fileSize = getFileSize(configFile);
    if (fileSize > 1024*1024) {
        ConfigReaderException(StringUtils::toString("Failed to read huge file " +
                                                    configFile + ". At " + __FILE__ + ":", __LINE__));
    }

    vector<char> fileBuffer(fileSize+1);
    fileBuffer[fileSize] = '\0';
    file.read(&fileBuffer[0], static_cast<std::streamsize>(fileSize));

    initFromString(&fileBuffer[0]);
}

void ConfigReader::initFromString(const std::string &config)
{
    for (const string &line : StringUtils::cropToStrings(config, '\n')) {
        auto sections = StringUtils::cropToStrings(line, ' ');
        if (sections.size() != 3 || sections[1] != "=") {
            ConfigReaderException(StringUtils::toString(string("Invalid format file") + ". At " + __FILE__ + ":", __LINE__));
        }
        _config.insert(std::make_pair(sections[0], sections[2]));
    }
}

ConfigReaderException::ConfigReaderException(const std::string &msg)
    : _msg(msg)
{
}

const char *ConfigReaderException::what() const noexcept
{
    return _msg.c_str();
}

ConfigNotFoundException::ConfigNotFoundException(const std::string &msg)
    : ConfigReaderException (msg)
{
}
