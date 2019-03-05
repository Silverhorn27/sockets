#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "Mutlithreading/All.h"
#include "Logger/Logger.h"
#include "Server.h"

class ConnectionHandler : public Runnable
{
    Logger _logger;
    int _socketfd;
    bool _requestStop;
public:
    ConnectionHandler(int fd);
    void run() override;
    void requestStop();

private:
    void onReceive();
    void onTimeout();
};

#endif // CONNECTION_HANDLER_H
