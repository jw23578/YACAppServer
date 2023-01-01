#ifndef HANDLERAPPUSERFETCHMESSAGEUPDATES_H
#define HANDLERAPPUSERFETCHMESSAGEUPDATES_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "databaselogics.h"

class HandlerAppUserFetchMessageUpdates : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserFetchMessageUpdates(PistacheServerInterface &serverInterface,
                                      DatabaseLogics &databaseLogics,
                                      LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method() override;
};

#endif // HANDLERAPPUSERFETCHMESSAGEUPDATES_H
