#ifndef HANDLERAPPUSERSPACE_H
#define HANDLERAPPUSERSPACE_H

#include "handlerloggedininterface.h"
#include "databaselogics.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerAppUserSpace : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserSpace(DatabaseLogics &databaseLogics,
                        PistacheServerInterface &serverInterface,
                        LoggedInAppUsersContainer &loggedInAppUsersContainer);

public:
    void method(CurrentContext &context) override;
};

#endif // HANDLERAPPUSERSPACE_H
