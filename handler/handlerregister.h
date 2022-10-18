#ifndef HANDLERREGISTER_H
#define HANDLERREGISTER_H

#include "interfaces/pistachehandlerinterface.h"

class HandlerRegister : public PistacheHandlerInterface
{
public:
    HandlerRegister(PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERREGISTER_H
