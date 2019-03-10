#include "ConnectionHandler.h"
#include "Utils/StringUtils.h"
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>
#include <poll.h>

ConnectionHandler::ConnectionHandler(std::unique_ptr<InteractorInterface> interactor, int fd)
    : _logger(StringUtils::toString("ConnectionHandler ", StringUtils::hexToString(this)))
    , _socketfd(fd)
    , _requestStop(false)
    , _interactor(std::move(interactor))
    , _buffer(DEFAULT_BUFFER_SIZE)
    , _state(State::Disconnect)
{

}

void ConnectionHandler::run()
{
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);
    const int timeoutInMsec = 5000;
    while (!_requestStop) {
        struct pollfd p;
        p.fd = _socketfd;
        p.events = POLLIN;

        int ret = poll(&p, 1, timeoutInMsec);
        if (ret > 0) {
            setState(State::Connect);
            if (p.revents & POLLIN) {
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

    ssize_t recv_size = recv(_socketfd, &_buffer[0], _buffer.size()-1, MSG_NOSIGNAL);
    if (recv_size <= 0) {
        if (recv_size < 0) {
            _logger.log(Logger::Debug, "error. recv_size is: ", recv_size);
	    }
        close(_socketfd);
    } else {
        _buffer[static_cast<size_t>(recv_size)] = '\0';
        _logger.log(Logger::Debug, &_buffer[0]);
        if (string(&_buffer[0]) == "exit\n") {
            _logger.log(Logger::Info, "The connection is torn");
            close(_socketfd);
            setState(State::Disconnect);
            requestStop();
        } else {
            _interactor->onReceiveData(&_buffer[0], static_cast<size_t>(recv_size));
        }
    }
}

void ConnectionHandler::onTimeout()
{
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);
}

void ConnectionHandler::requestStop() 
{
    _requestStop = true;
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
