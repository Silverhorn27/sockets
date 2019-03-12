#ifndef THREAD_H
#define THREAD_H

#include "Runnable.h"
#include <memory>
#include <future>
#include <atomic>

class Thread {
private:
    Runnable *_rawRunnable;
    Runnable::Ptr _runnable;
    std::atomic_bool _isUsed;
    std::atomic_bool _isStopped;
    std::atomic_bool _isActive;
    std::future<void> _thrFuture;
    std::atomic_bool _threadIsOwnerOfTheRunnable;
    std::mutex _setMutex;

private:
    void mainLoop();

public:
    Thread();
    using Ptr = std::unique_ptr<Thread>;
    static void sleep(size_t msec);
    void setRunnable(Runnable::Ptr &&runnable);
    void setRunnable(Runnable *runnable, bool threadIsOwnerOfTheRunnable);

    bool isWaiting() const { return !_isUsed && !_isStopped; }
    bool isUsed() const { return _isUsed; }
    bool isStopped() const { return _isStopped; }
    void stopLoop() { _isActive = false; }

};

#endif // THREAD_H
