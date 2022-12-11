#ifndef HANDLERAPPUSERUPDATEPROFILE_H
#define HANDLERAPPUSERUPDATEPROFILE_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "databaselogicappuser.h"

class HandlerAppUserUpdateProfile : public HandlerLoggedInInterface
{
    DatabaseLogicAppUser &databaseLogicAppUser;
public:
    HandlerAppUserUpdateProfile(PistacheServerInterface &serverInterface,
                                DatabaseLogicAppUser &databaseLogicAppUser,
                                LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method() override;
};

#endif // HANDLERAPPUSERUPDATEPROFILE_H
