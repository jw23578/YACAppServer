#ifndef HANDLERAPPUSERFETCHPROFILE_H
#define HANDLERAPPUSERFETCHPROFILE_H

#include "handlerloggedininterface.h"
#include "databaselogic/databaselogics.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerAppUserFetchProfile : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserFetchProfile(DatabaseLogics &databaseLogics,
                               PistacheServerInterface &serverInterface,
                               LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method(CurrentContext &context) override;
};

#endif // HANDLERAPPUSERFETCHPROFILE_H
