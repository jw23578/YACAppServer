#ifndef HANDLERREGISTERUSER_H
#define HANDLERREGISTERUSER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogic.h"
#include "emaillogic.h"

class HandlerRegisterUser : public PistacheHandlerInterface
{
    DatabaseLogic &databaseLogic;
    EMailLogic &emailLogic;
public:
    HandlerRegisterUser(DatabaseLogic &databaseLogic,
                    EMailLogic &emailLogic,
                    PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERREGISTERUSER_H
