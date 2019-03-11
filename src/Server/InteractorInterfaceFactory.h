#ifndef INTERACTOR_INTERFACE_FACTORY_H
#define INTERACTOR_INTERFACE_FACTORY_H
#include "InteractorInterface.h"
#include <memory>

class InteractorInterfaceFactory
{
public:
    virtual ~InteractorInterfaceFactory() = default;
    virtual std::unique_ptr<InteractorInterface> createInteractorObject() = 0;
};

#endif // INTERACTOR_INTERFACE_FACTORY_H
