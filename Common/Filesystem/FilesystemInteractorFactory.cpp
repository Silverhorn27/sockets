#include "FilesystemInteractorFactory.h"
#include "FilesystemInteractor.h"

FilesystemInteractorFactory::FilesystemInteractorFactory()
    : _logger(StringUtils::toString("FilesystemInteractorFactory ", StringUtils::hexToString(this)))
{
}

std::unique_ptr<InteractorInterface> FilesystemInteractorFactory::createInteractorObject()
{
    return std::unique_ptr<InteractorInterface>(new FilesystemInterator());
}
