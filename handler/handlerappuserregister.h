#ifndef HANDLERAPPUSERREGISTER_H
#define HANDLERAPPUSERREGISTER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogics.h"
#include "emaillogic.h"
#include "yacappservermethodnames.h"

class HandlerAppUserRegister : public PistacheHandlerInterface
{
    YACAPPServerMethodNames methodNames;
    DatabaseLogics &databaseLogics;
    EMailLogic &emailLogic;
public:
    HandlerAppUserRegister(DatabaseLogics &databaseLogics,
                           EMailLogic &emailLogic,
                           PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERAPPUSERREGISTER_H
