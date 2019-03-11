#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <mutex>
#include <vector>
#include <cstddef>
#include "Thread.h"
#include "Runnable.h"
#include "MultithreadingConstant.h"

class ThreadPool {
private:
    static ThreadPool *_defaultPool;
    std::vector<Thread::Ptr> _threads;
    std::atomic_bool _stopping;
    std::mutex _sync;

private:
    Thread::Ptr &getFreeThread();

public:
    ThreadPool(size_t capacity = DEFAULT_CAPACITY_THREAD_POOL);
    ~ThreadPool();

    class ExhausedThreads : public std::exception {
        std::string _message;
    public:
        ExhausedThreads(const char *msg = "ho have free threads") : _message(msg) {}
        const char *what() const throw() override { return _message.c_str(); }
    };

    static ThreadPool *defaultPool();
    bool start(Runnable::Ptr &&runObj, bool waitIfNoFree = false);
    bool isStopping() const { return _stopping; }
    void stopAllThreads();
    void waitAll();
};

#endif // THREAD_POOL_H
