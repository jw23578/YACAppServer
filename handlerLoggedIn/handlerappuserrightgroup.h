#ifndef HANDLERAPPUSERRIGHTGROUP_H
#define HANDLERAPPUSERRIGHTGROUP_H

#include "handlerloggedininterface.h"
#include "databaselogics.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerAppUserRightGroup : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserRightGroup(DatabaseLogics &databaseLogics,
                             PistacheServerInterface &serverInterface,
                             LoggedInAppUsersContainer &loggedInAppUsersContainer);

public:
    void method(CurrentContext &context) override;
};

#endif // HANDLERAPPUSERRIGHTGROUP_H
