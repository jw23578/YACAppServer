#ifndef HANDLERAPPUSERUPDATEPROFILE_H
#define HANDLERAPPUSERUPDATEPROFILE_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "databaselogics.h"

class HandlerAppUserUpdateProfile : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserUpdateProfile(PistacheServerInterface &serverInterface,
                                DatabaseLogics &databaseLogics,
                                LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method() override;
};

#endif // HANDLERAPPUSERUPDATEPROFILE_H
