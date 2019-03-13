#ifndef RUNNABLE_H
#define RUNNABLE_H
#include <memory>

class Runnable {
public:
    using Ptr = std::unique_ptr<Runnable>;
    Runnable() = default;
    virtual ~Runnable() = default;
    virtual void run() = 0;
};

#endif // RUNNABLE_H
