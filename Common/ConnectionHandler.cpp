#include "ConnectionHandler.h"
#include "Utils/StringUtils.h"
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <strings.h>
#include <poll.h>

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
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);
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

void ConnectionHandler::onReceive()
{
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);

    auto recv_size = recv(_socketfd, &_buffer[0], _buffer.size()-1, MSG_NOSIGNAL);
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
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);
}

void ConnectionHandler::requestStop() 
{
    _logger.log(Logger::Debug, __PRETTY_FUNCTION__);
    _requestStop = true;
    close(_socketfd);
    setState(State::Disconnect);
}

void ConnectionHandler::setState(State state) 
{
    _state = state;
}

bool ConnectionHandler::connectionActive() 
{
    return _state == State::Active;
}

bool ConnectionHandler::setBufferSize(size_t bufferSize)
{
    if (bufferSize > MAX_BUFFER_SIZE) {
        _logger.log(Logger::Error, "Exceeded MAX_BUFFER_SIZE: ", bufferSize);
        return false;
    }
    _buffer.resize(bufferSize);
    return true;
}

char ConnectionHandler::getStatus()
{
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
    auto ret = send(_socketfd, reinterpret_cast<const void *>(data), lenght, MSG_NOSIGNAL);
    if (ret <= 0) {
        if (ret < 0) {
            _logger.log(Logger::Error, "send() was returned status ", ret);
        }
        requestStop();
    }

    return static_cast<int>(ret);
}
