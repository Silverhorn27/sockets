#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "Mutlithreading/All.h"
#include "Logger/Logger.h"
#include "InteractorInterface.h"


class ConnectionHandler : public Runnable
{
public:
    enum State : int {
            Active,
            Inactive,
            Disconnect,
            Connect
        };

private:
    Logger _logger;
    int _socketfd;
    bool _requestStop;
    std::unique_ptr<InteractorInterface> _interactor;
    Buffer _buffer;
    State _state;

public:
    ConnectionHandler(std::unique_ptr<InteractorInterface> interactor, int fd);
    void run() override;
    void requestStop();
    bool connectionActive();
    bool setBufferSize(size_t bufferSize);
    char getStatus();

private:
    void onReceive();
    void onTimeout();
    void setState(State state);
};

#endif // CONNECTION_HANDLER_H
