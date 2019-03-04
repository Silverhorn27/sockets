#include "Thread.h"
#include "MultithreadingConstant.h"

Thread::Thread() :
    _runnable(nullptr),
    _isUsed(false),
    _isStopped(false),
    _isActive(true)

{
    _thrFuture = std::async(std::launch::async,
                           [&](){ mainLoop(); });
}

void Thread::setRunnable(Runnable::Ptr &&runnable) {
    _isUsed = true;
    _runnable = std::move(runnable);
}

void Thread::mainLoop() {
    while (_isActive) {
        if (_runnable) {
            _runnable->run();
            _runnable = nullptr;
        }

        _isUsed.store(false);
        Thread::sleep(DELAY_THREAD_MSEC);
    }
    _isStopped.store(true);
}

void Thread::sleep(size_t msec) {
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}
