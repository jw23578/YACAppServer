#ifndef HANDLERREGISTERUSER_H
#define HANDLERREGISTERUSER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogic.h"
#include "emaillogic.h"

class HandlerRegisterUser : public PistacheHandlerInterface
{
    DatabaseLogicUserAndApp &databaseLogicUserAndApp;
    EMailLogic &emailLogic;
public:
    HandlerRegisterUser(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                    EMailLogic &emailLogic,
                    PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERREGISTERUSER_H
