#ifndef HANDLERUPLOADAPP_H
#define HANDLERUPLOADAPP_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerUploadApp : public HandlerLoggedInInterface
{
public:
    HandlerUploadApp(ORMPersistenceInterface &opi,
                     PistacheServerInterface &serverInterface,
                     LoggedInAppUsersContainer &loggedInAppUsersContainer);

    // PistacheHandlerInterface interface
public:
    void method(CurrentContext &context) override;
};

#endif // HANDLERUPLOADAPP_H
