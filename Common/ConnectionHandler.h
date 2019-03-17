#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "Mutlithreading/All.h"
#include "Logger/Logger.h"
#include "Factory/InteractorInterface.h"


using DeleterCallback = std::function<void(class ConnectionHandler *)>;

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
    DeleterCallback _deleteCallback;

public:
    ~ConnectionHandler() override;
    ConnectionHandler(std::unique_ptr<InteractorInterface> interactor, int fd, DeleterCallback callback);
    void run() override;
    void requestStop();
    bool connectionActive();
    bool setBufferSize(size_t bufferSize);
    char getStatus();
    int sendData(const char *data, size_t lenght);
    int available();

private:
    int send(const char *data, size_t lenght);
    int receiveData(char *data, size_t lenght);
    void onReceive();
    void onTimeout();
    void setState(State state);
    int fullySend(const void *data, size_t lenght);
    int fullyRecv(void *data, size_t lenght);
};

#endif // CONNECTION_HANDLER_H
