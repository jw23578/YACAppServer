#ifndef HANDLERAPPUSERGETWORKTIMESTATE_H
#define HANDLERAPPUSERGETWORKTIMESTATE_H

#include "handlerloggedininterface.h"
#include "databaselogics.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerAppUserGetWorktimeState : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserGetWorktimeState(DatabaseLogics &databaseLogics,
                                   PistacheServerInterface &serverInterface,
                                   LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method(CurrentContext &context) override;
};

#endif // HANDLERAPPUSERGETWORKTIMESTATE_H
