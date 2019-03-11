#include "ThreadPool.h"
#include "MultithreadingConstant.h"
#include <iostream>

ThreadPool *ThreadPool::_defaultPool(nullptr);

ThreadPool::ThreadPool(size_t capacity)
    : _threads(0),
      _stopping(false)
{
    _threads.reserve(capacity);
    for (size_t i = 0; i < capacity; ++i) {
        _threads.push_back(Thread::Ptr(new Thread()));
    }
}

Thread::Ptr &ThreadPool::getFreeThread()
{
    if (!_stopping) {
        std::unique_lock locker(_sync);
        for (Thread::Ptr &thread : _threads) {
            if (!thread->isUsed()) {
                return thread;
            }
        }
    }

    throw ExhausedThreads();
}

ThreadPool::~ThreadPool()
{
    stopAllThreads();
    waitAll();
}

ThreadPool *ThreadPool::defaultPool()
{
    if (!_defaultPool)
        _defaultPool = new ThreadPool();

    return _defaultPool;
}

bool ThreadPool::start(Runnable::Ptr &&runObj, bool waitIfNoFree)
{
    do {
        try {
            getFreeThread()->setRunnable(std::move(runObj));
            break;
        } catch (ExhausedThreads &e) {
            if (waitIfNoFree) {
                Thread::sleep(2);
            }
            else {
                std::cerr << e.what();
                return false;
            }
        }
    } while(waitIfNoFree);

    return true;
}

void ThreadPool::stopAllThreads()
{
    _stopping.store(true);
    for (Thread::Ptr &p : _threads)
        p->stopLoop();
}

void ThreadPool::waitAll()
{
    do {
        for (Thread::Ptr &p : _threads) {
            if (p->isUsed()) {
                continue;
            }
        }
        break;
    } while (true);
}
