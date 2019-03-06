#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "Mutlithreading/All.h"
#include "Logger/Logger.h"

class ConnectionHandler : public Runnable
{
public:
    enum State : int {
            Active,
            Inactive,
            Disconect
        };

private:
    Logger _logger;
    int _socketfd;
    bool _requestStop;
    Buffer _buffer;
    State _state;

public:
    ConnectionHandler(int fd);
    void run() override;
    void requestStop();
    bool connectionActive();
    void setState(State state);
    void setBufferSize(size_t bufferSize);

private:
    void onReceive();
    void onTimeout();
};

#endif // CONNECTION_HANDLER_H
