#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "Mutlithreading/All.h"
#include "Logger/Logger.h"
#include "InteractorInterface.h"

class ConnectionHandler : public Runnable
{
    Logger _logger;
    int _socketfd;
    bool _requestStop;
    InteractorInterface *_interactor;

public:
    ConnectionHandler(InteractorInterface *interactor, int fd);
    void run() override;
    void requestStop();

private:
    void onReceive();
    void onTimeout();
};

#endif // CONNECTION_HANDLER_H
