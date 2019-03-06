#include "FilesystemInteractorFactory.h"
#include "FilesystemInteractor.h"

FilesystemInteractorFactory::FilesystemInteractorFactory()
    : _logger(StringUtils::toString("FilesystemInteractorFactory ", StringUtils::hexToString(this)))
{
}

InteractorInterface *FilesystemInteractorFactory::createInteractorObject()
{
    return new FilesystemInterator();
}
