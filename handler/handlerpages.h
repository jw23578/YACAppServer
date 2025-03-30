#ifndef HANDLERPAGES_H
#define HANDLERPAGES_H

#include "interfaces/pistachehandlerinterface.h"

class HandlerPages : public PistacheHandlerInterface
{
public:
    HandlerPages(PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERPAGES_H
