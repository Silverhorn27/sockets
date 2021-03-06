#ifndef FILESYSTEM_INTERACTOR_FACTORY_H
#define FILESYSTEM_INTERACTOR_FACTORY_H

#include "Factory/InteractorInterfaceFactory.h"
#include "Logger/Logger.h"

class FilesystemInteractorFactory : public InteractorInterfaceFactory
{
    Logger _logger;

public:
    FilesystemInteractorFactory();
    std::unique_ptr<InteractorInterface> createInteractorObject() override;
};

#endif // FILESYSTEM_INTERACTOR_FACTORY_H
