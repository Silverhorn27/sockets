#include "ConnectionHandler.h"
#include "Utils/StringUtils.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <strings.h>
#include <poll.h>
#include <csignal>

ConnectionHandler::~ConnectionHandler()
{
    _deleteCallback(this);
}

ConnectionHandler::ConnectionHandler(std::unique_ptr<InteractorInterface> interactor, int fd, DeleterCallback callback)
    : _logger(StringUtils::toString("ConnectionHandler ", StringUtils::hexToString(this)))
    , _socketfd(fd)
    , _requestStop(false)
    , _interactor(std::move(interactor))
    , _buffer(DEFAULT_BUFFER_SIZE)
    , _state(State::Disconnect)
    , _deleteCallback(callback)
{

}

void ConnectionHandler::run()
{
    TRACE_FUNCTION();
    const int timeoutInMsec = 1000;
    while (!_requestStop) {
        struct pollfd pollData{};
        bzero(&pollData, sizeof(pollData));
        pollData.fd = _socketfd;
        pollData.events = POLLIN;

        int ret = poll(&pollData, 1, timeoutInMsec);
        if (ret > 0) {
            setState(State::Connect);
            if (pollData.revents & POLLIN) {
                setState(State::Active);
                onReceive();
            }
        } else {
            setState(State::Inactive);
            _logger.log(Logger::Debug, "timeout");
            onTimeout();
        }
        // check poll and doing onReceive or onTimeout
        Thread::sleep(10);
    }
}

int ConnectionHandler::receiveData(char *data, size_t lenght)
{
    TRACE_FUNCTION();
    uint16_t NBOLenght = 0;
    if (fullyRecv(&NBOLenght, sizeof (NBOLenght)) == -1) {
        return -1;
    }
    NBOLenght = ntohs(NBOLenght);

    if (NBOLenght > lenght) {
        _logger.log(Logger::Critical, "NBOLenght > lenght [", NBOLenght, ',', lenght, "]. ",
                    "At ", __FILE__, ':', __LINE__);
        std::raise(SIGABRT);
    }

    _logger.log(Logger::Info, "Recv msg size ", NBOLenght);
    return fullyRecv(data, NBOLenght);
}

void ConnectionHandler::onReceive()
{
    TRACE_FUNCTION();
    auto recv_size = receiveData(&_buffer[0], _buffer.size()-1);
    if (recv_size <= 0) {
        if (recv_size < 0) {
            _logger.log(Logger::Debug, "error. recv_size is: ", recv_size);
	    }
        requestStop();
    } else {
        _buffer[static_cast<size_t>(recv_size)] = '\0';
        _interactor->onReceiveData(&_buffer[0], static_cast<size_t>(recv_size));
    }
}

void ConnectionHandler::onTimeout()
{
    TRACE_FUNCTION();
}

void ConnectionHandler::requestStop() 
{
    TRACE_FUNCTION();
    _requestStop = true;
    close(_socketfd);
    setState(State::Disconnect);
}

void ConnectionHandler::setState(State state) 
{
    TRACE_FUNCTION();
    _state = state;
}

bool ConnectionHandler::connectionActive() 
{
    TRACE_FUNCTION();
    return _state == State::Active;
}

bool ConnectionHandler::setBufferSize(size_t bufferSize)
{
    TRACE_FUNCTION();
    if (bufferSize > MAX_BUFFER_SIZE) {
        _logger.log(Logger::Error, "Exceeded MAX_BUFFER_SIZE: ", bufferSize);
        return false;
    }
    _buffer.resize(bufferSize);
    return true;
}

char ConnectionHandler::getStatus()
{
    TRACE_FUNCTION();
    switch (_state)
    {
        case Active:     return 'A';
        case Inactive:   return 'I';
        case Connect:    return 'C';
        case Disconnect: return 'D';
    }
    return 'U';
}

int ConnectionHandler::sendData(const char *data, size_t lenght)
{
    TRACE_FUNCTION();
    auto ret = send(data, lenght);
    if (ret <= 0) {
        if (ret < 0) {
            _logger.log(Logger::Error, "send() was returned status ", ret);
        }
        requestStop();
    }

    return static_cast<int>(ret);
}

int ConnectionHandler::available()
{
    TRACE_FUNCTION();
    int value = 0;
    int err = ioctl(_socketfd, FIONREAD, &value);

    return (err ? err : value);
}

int ConnectionHandler::send(const char *data, size_t lenght)
{
    TRACE_FUNCTION();
    // Network Bytes Order
    _logger.log(Logger::Info, "Send msg size ", lenght);
    uint16_t NBOLenght = htons(static_cast<uint16_t>(lenght));
    // Send header
    if (fullySend(&NBOLenght, sizeof(NBOLenght)) == -1) {
        return -1;
    }
    // Send body
    return fullySend(data, lenght);
}

int ConnectionHandler::fullySend(const void *data, size_t lenght)
{
    TRACE_FUNCTION();
    size_t sent = 0;
    ssize_t lastSent = 0;
    while (sent != lenght) {
        lastSent = ::send(_socketfd, reinterpret_cast<const char*>(data) + sent, lenght, MSG_NOSIGNAL);
        if (lastSent == -1) {
            return -1;
        }
        sent += static_cast<size_t>(lastSent);
    }
    return static_cast<int>(sent);
}

int ConnectionHandler::fullyRecv(void *data, size_t lenght)
{
    TRACE_FUNCTION();
    size_t received = 0;
    ssize_t lastReceived = 0;
    while (received != lenght) {
        lastReceived = ::recv(_socketfd, reinterpret_cast<char *>(data) + received, lenght, MSG_NOSIGNAL);
        if (lastReceived == -1) {
            return -1;
        }
        received += static_cast<size_t>(lastReceived);
    }
    return static_cast<int>(received);
}
