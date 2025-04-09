#ifndef HANDLERVERIFYUSER_H
#define HANDLERVERIFYUSER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogicuserandapp.h"

class HandlerVerifyUser : public PistacheHandlerInterface
{
    DatabaseLogicUserAndApp &databaseLogicUserAndApp;
public:
    HandlerVerifyUser(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                      PistacheServerInterface &serverInterface);
    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERVERIFYUSER_H
