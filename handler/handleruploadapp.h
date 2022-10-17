#ifndef HANDLERUPLOADAPP_H
#define HANDLERUPLOADAPP_H

#include <pistachehandlerinterface.h>

class HandlerUploadAPP : public PistacheHandlerInterface
{
public:
    HandlerUploadAPP(Pistache::Rest::Router &router);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERUPLOADAPP_H
