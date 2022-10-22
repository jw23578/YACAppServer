#ifndef HANDLERUPLOADAPP_H
#define HANDLERUPLOADAPP_H

#include "handlerloggedininterface.h"
#include "databaselogic.h"

class HandlerUploadApp : public HandlerLoggedInInterface
{
    DatabaseLogic &databaseLogic;
public:
    HandlerUploadApp(DatabaseLogic &databaseLogic,
                     PistacheServerInterface &serverInterface,
                     LoggedInUsersContainer &loggedInUsersContainer);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERUPLOADAPP_H
