#ifndef HANDLERLOGINUSER_H
#define HANDLERLOGINUSER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogic.h"

class HandlerLoginUser : public PistacheHandlerInterface
{
    DatabaseLogicUserAndApp &databaseLogicUserAndApp;
public:
    HandlerLoginUser(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                     PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERLOGINUSER_H
