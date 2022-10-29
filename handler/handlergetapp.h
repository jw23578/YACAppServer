#ifndef HANDLERGETAPP_H
#define HANDLERGETAPP_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogic.h"

class HandlerGetAPP : public PistacheHandlerInterface
{
    DatabaseLogic &databaseLogic;

public:
    HandlerGetAPP(DatabaseLogic &databaseLogic,
                  PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERGETAPP_H
