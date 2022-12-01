#ifndef HANDLERAPPUSERREQUESTUPDATEPASSWORD_H
#define HANDLERAPPUSERREQUESTUPDATEPASSWORD_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogicappuser.h"
#include "emaillogic.h"

class HandlerAppUserRequestUpdatePassword : public PistacheHandlerInterface
{
    DatabaseLogicAppUser &databaseLogicAppUser;
    EMailLogic &emailLogic;
public:
    HandlerAppUserRequestUpdatePassword(DatabaseLogicAppUser &databaseLogicAppUser,
                                        EMailLogic &emailLogic,
                                        PistacheServerInterface &serverInterface);
    void method() override;
};

#endif // HANDLERAPPUSERREQUESTUPDATEPASSWORD_H
