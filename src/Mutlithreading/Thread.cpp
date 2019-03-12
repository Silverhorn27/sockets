#include "Thread.h"
#include "MultithreadingConstant.h"

Thread::Thread() :
    _rawRunnable(nullptr),
    _runnable(nullptr),
    _isUsed(false),
    _isStopped(false),
    _isActive(true),
    _threadIsOwnerOfTheRunnable(false)
{
    _thrFuture = std::async(std::launch::async,
                           [&](){ mainLoop(); });
}

void Thread::setRunnable(Runnable::Ptr &&runnable)
{
    std::unique_lock locker(_setMutex);

    _threadIsOwnerOfTheRunnable = true;
    _isUsed = true;
    _runnable = std::move(runnable);
}

void Thread::setRunnable(Runnable *runnable, bool threadIsOwnerOfTheRunnable)
{
    std::unique_lock locker(_setMutex);

    _threadIsOwnerOfTheRunnable = threadIsOwnerOfTheRunnable;

    if (_threadIsOwnerOfTheRunnable) {
        _runnable.reset(runnable);
    } else {
        _rawRunnable = runnable;
    }

    _isUsed = true;
}

template <typename TRunnable>
static void mainLoopImpl(TRunnable runnable) {
    if (runnable) {
        runnable->run();
        runnable = nullptr;
    }
}

void Thread::mainLoop()
{
    while (_isActive) {
        {
            std::unique_lock locker(_setMutex);
            if (_threadIsOwnerOfTheRunnable) {
                // The _rawRunnable will auto deleted.
                mainLoopImpl(std::move(_runnable));
            } else {
                // The _rawRunnable will not deleted.
                mainLoopImpl<Runnable*&>(_rawRunnable);
            }
        }
        _isUsed.store(false);
        Thread::sleep(DELAY_THREAD_MSEC);
    }
    _isStopped.store(true);
}

void Thread::sleep(size_t msec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}
