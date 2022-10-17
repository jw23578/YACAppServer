#ifndef HANDLERREGISTER_H
#define HANDLERREGISTER_H

#include <pistachehandlerinterface.h>

class HandlerRegister : public PistacheHandlerInterface
{
public:
    HandlerRegister(Pistache::Rest::Router &router);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERREGISTER_H
