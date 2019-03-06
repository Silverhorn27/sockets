#ifndef INTERACTOR_INTERFACE_H
#define INTERACTOR_INTERFACE_H
#include <cstdint>
using std::size_t;

class InteractorInterface {
public:
    virtual ~InteractorInterface() = default;
    virtual void onReceiveData(char *data, size_t len) = 0;
    virtual void onSocketError() = 0;
    virtual void onSocketTimeout() = 0;
};

#endif // INTERACTOR_INTERFACE_H
