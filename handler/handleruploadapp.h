#ifndef HANDLERUPLOADAPP_H
#define HANDLERUPLOADAPP_H

#include "interfaces/pistachehandlerinterface.h"

class HandlerUploadAPP : public PistacheHandlerInterface
{
public:
    HandlerUploadAPP(PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERUPLOADAPP_H
