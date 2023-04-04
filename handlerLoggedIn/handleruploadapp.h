#ifndef HANDLERUPLOADAPP_H
#define HANDLERUPLOADAPP_H

#include "handlerloggedininterface.h"
#include "databaselogicuserandapp.h"
#include "loggedincontainer/loggedinuserscontainer.h"
#include "orm_implementions/t0027_app_images.h"

class HandlerUploadApp : public HandlerLoggedInInterface
{
    t0027_app_images t0027;
    DatabaseLogicUserAndApp &dlua;
public:
    HandlerUploadApp(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                     PistacheServerInterface &serverInterface,
                     LoggedInUsersContainer &loggedInUsersContainer);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERUPLOADAPP_H
