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
{

}

void ConnectionHandler::run()
{
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);
    const int timeoutInMsec = 10000;
    while (!_requestStop) {
        struct pollfd p;
        p.fd = _socketfd;
        p.events = POLLIN;

        int ret = poll(&p, _socketfd, timeoutInMsec);
        if (ret > 0) {
            if (p.revents & POLLIN) {
                onReceive();
            } else {
                onTimeout();
            }
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
    char buffer[1024];
    int recv_size = recv(_socketfd, buffer, sizeof(buffer), MSG_NOSIGNAL);
    if (recv_size <= 0) {
        if (recv_size < 0) {
            _logger.log(Logger::Debug, "error. recv_size is: ", recv_size);
	    }
        close(_socketfd);
    } else {
        _logger.log(Logger::Debug, buffer);
        if (buffer == "close") {
            requestStop();
        }
        send(_socketfd, buffer, recv_size, MSG_NOSIGNAL);
    }
}

void ConnectionHandler::onTimeout()
{
    _logger.log(Logger::Trace, __PRETTY_FUNCTION__);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
}

void ConnectionHandler::requestStop() {
    _requestStop = true;
}
