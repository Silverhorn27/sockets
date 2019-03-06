#include "ConnectionHandler.h"
#include "Utils/StringUtils.h"
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>
#include <poll.h>

ConnectionHandler::ConnectionHandler(int fd)
    : _logger(StringUtils::toString("ConnectionHandler ", StringUtils::hexToString(this)))
    , _socketfd(fd)
    , _requestStop(false)
    , _buffer(DEFAULT_BUFFER_SIZE)
    , _state(State::Disconect)
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
            if (p.revents & POLLIN) {
                _state = State::Active;
                onReceive();
            }
        } else {
            _state = State::Inactive;
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
    {
        char l[]{"This is some big text. for something checks"};

    }
    _buffer.resize(64);
    ssize_t recv_size = recv(_socketfd, &_buffer[0], sizeof(_buffer)-1, MSG_NOSIGNAL);
    if (recv_size <= 0) {
        if (recv_size < 0) {
            _logger.log(Logger::Debug, "error. recv_size is: ", recv_size);
	    }
        close(_socketfd);
    } else {
        _buffer[recv_size] = '\0';
        _logger.log(Logger::Debug, &_buffer[0]);
        if (string(&_buffer[0]) == "close") {
            _logger.log(Logger::Info, "The connection is torn");
            close(_socketfd);
            _state = State::Disconect;
            requestStop();
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
    return _state == State::Active ? true : false;
}

void ConnectionHandler::setBufferSize(size_t bufferSize)
{
    if (bufferSize <= MAX_BUFFER_SIZE) {
        _buffer.resize(bufferSize);
    } else {
        _logger.log(Logger::Error, "Exceeded MAX_BUFFER_SIZE: ", bufferSize);
    }
}