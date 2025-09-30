#ifndef HANDLERAPPUSERFETCHIMAGE_H
#define HANDLERAPPUSERFETCHIMAGE_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "databaselogics.h"

class HandlerAppUserFetchImage : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserFetchImage(PistacheServerInterface &serverInterface,
                             DatabaseLogics &databaseLogics,
                             LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method(CurrentContext &context) override;
};

#endif // HANDLERAPPUSERFETCHIMAGE_H
