#ifndef HANDLERAPPUSERINSERTWORKTIME_H
#define HANDLERAPPUSERINSERTWORKTIME_H

#include "handlerloggedininterface.h"
#include "databaselogics.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerAppUserInsertWorktime : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserInsertWorktime(DatabaseLogics &databaseLogics,
                                 PistacheServerInterface &serverInterface,
                                 LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method() override;
};

#endif // HANDLERAPPUSERINSERTWORKTIME_H
