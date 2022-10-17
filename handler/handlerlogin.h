#ifndef HANDLERLOGIN_H
#define HANDLERLOGIN_H

#include <pistachehandlerinterface.h>

class HandlerLogin : public PistacheHandlerInterface
{
public:
    HandlerLogin(Pistache::Rest::Router &router);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERLOGIN_H
