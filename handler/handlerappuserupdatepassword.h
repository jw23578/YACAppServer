#ifndef HANDLERAPPUSERUPDATEPASSWORD_H
#define HANDLERAPPUSERUPDATEPASSWORD_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogicappuser.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerAppUserUpdatePassword : public PistacheHandlerInterface
{
    DatabaseLogicAppUser &databaseLogicAppUser;
    LoggedInAppUsersContainer &loggedInAppUsersContainer;
public:
    HandlerAppUserUpdatePassword(DatabaseLogicAppUser &databaseLogicAppUser,
                                 LoggedInAppUsersContainer &loggedInAppUsersContainer,
                                 PistacheServerInterface &serverInterface);
    void method() override;
};

#endif // HANDLERAPPUSERUPDATEPASSWORD_H
