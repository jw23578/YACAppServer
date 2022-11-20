#ifndef HANDLERAPPUSERREGISTER_H
#define HANDLERAPPUSERREGISTER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogicappuser.h"
#include "emaillogic.h"


class HandlerAppUserRegister : public PistacheHandlerInterface
{
    DatabaseLogicAppUser &databaseLogicAppUser;
    EMailLogic &emailLogic;
public:
    HandlerAppUserRegister(DatabaseLogicAppUser &databaseLogicAppUser,
                           EMailLogic &emailLogic,
                           PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERAPPUSERREGISTER_H
