#ifndef HANDLERAPP_H
#define HANDLERAPP_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogicuserandapp.h"

class HandlerAPP : public PistacheHandlerInterface
{
    DatabaseLogicUserAndApp &dlua;

public:
    HandlerAPP(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                  PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERAPP_H
