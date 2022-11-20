#ifndef HANDLERVERIFYUSER_H
#define HANDLERVERIFYUSER_H

#include <pistachehandlerinterface.h>
#include "databaselogic.h"

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
