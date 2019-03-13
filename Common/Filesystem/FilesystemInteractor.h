#ifndef FILESYSTEM_INTERACTOR_H
#define FILESYSTEM_INTERACTOR_H

#include "Factory/InteractorInterface.h"
#include "Logger/Logger.h"

class FilesystemInterator : public InteractorInterface
{
    Logger _logger;

public:
    FilesystemInterator();
    void onReceiveData(char *data, size_t countBytes) override;
    void onSocketError() override;
    void onSocketTimeout() override;
};

#endif // FILESYSTEM_INTERACTOR_H
