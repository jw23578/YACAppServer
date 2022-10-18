#ifndef HANDLERLOGIN_H
#define HANDLERLOGIN_H

#include "interfaces/pistachehandlerinterface.h"

class HandlerLogin : public PistacheHandlerInterface
{
public:
    HandlerLogin(PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERLOGIN_H
