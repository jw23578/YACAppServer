#ifndef HANDLERAPPUSERUPDATEMESSAGESTATES_H
#define HANDLERAPPUSERUPDATEMESSAGESTATES_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "databaselogics.h"
class HandlerAppUserUpdateMessageStates : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserUpdateMessageStates(PistacheServerInterface &serverInterface,
                                      DatabaseLogics &databaseLogics,
                                      LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method() override;
};

#endif // HANDLERAPPUSERUPDATEMESSAGESTATES_H
