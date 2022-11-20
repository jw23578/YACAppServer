#ifndef HANDLERGETAPP_H
#define HANDLERGETAPP_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogicuserandapp.h"

class HandlerGetAPP : public PistacheHandlerInterface
{
    DatabaseLogicUserAndApp &databaseLogicUserAndApp;

public:
    HandlerGetAPP(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                  PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERGETAPP_H
