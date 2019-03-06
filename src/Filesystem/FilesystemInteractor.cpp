#include "FilesystemInteractor.h"

FilesystemInterator::FilesystemInterator()
    : _logger(StringUtils::toString("FilesystemInteractor ", StringUtils::hexToString(this)))
{
}

void FilesystemInterator::onReceiveData(char *data, size_t countBytes)
{
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);
}

void FilesystemInterator::onSocketError()
{
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);
}

void FilesystemInterator::onSocketTimeout()
{
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);
}
