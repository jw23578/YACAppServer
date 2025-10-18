#ifndef HANDLERAPP_H
#define HANDLERAPP_H

#include "interfaces/pistachehandlerinterface.h"

class HandlerAPP : public PistacheHandlerInterface
{
public:
    HandlerAPP(ORMPersistenceInterface &opi,
               PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method(CurrentContext &context) override;
};

#endif // HANDLERAPP_H
