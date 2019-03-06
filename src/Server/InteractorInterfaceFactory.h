#ifndef INTERACTOR_INTERFACE_FACTORY_H
#define INTERACTOR_INTERFACE_FACTORY_H
#include "InteractorInterface.h"

class InteractorInterfaceFactory
{
public:
    virtual ~InteractorInterfaceFactory() = default;
    virtual InteractorInterface* createInteractorObject() = 0;
};

#endif // INTERACTOR_INTERFACE_FACTORY_H
