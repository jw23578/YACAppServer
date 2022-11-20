#ifndef HANDLERUPLOADAPP_H
#define HANDLERUPLOADAPP_H

#include "handlerloggedininterface.h"
#include "databaselogicuserandapp.h"

class HandlerUploadApp : public HandlerLoggedInInterface
{
    DatabaseLogicUserAndApp &databaseLogicUserAndApp;
public:
    HandlerUploadApp(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                     PistacheServerInterface &serverInterface,
                     LoggedInUsersContainer &loggedInUsersContainer);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERUPLOADAPP_H
