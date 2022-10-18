#ifndef HANDLERGETAPP_H
#define HANDLERGETAPP_H

#include "interfaces/pistachehandlerinterface.h"

class HandlerGetAPP : public PistacheHandlerInterface
{
public:
    HandlerGetAPP(PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERGETAPP_H
