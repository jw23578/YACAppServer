#ifndef HANDLERGETAPP_H
#define HANDLERGETAPP_H

#include <pistachehandlerinterface.h>

class HandlerGetAPP : public PistacheHandlerInterface
{
public:
    HandlerGetAPP(Pistache::Rest::Router &router);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERGETAPP_H
