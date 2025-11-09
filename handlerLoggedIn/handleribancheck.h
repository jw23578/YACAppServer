#ifndef HANDLERIBANCHECK_H
#define HANDLERIBANCHECK_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerIBANCheck : public HandlerLoggedInInterface
{
public:
    HandlerIBANCheck(ORMPersistenceInterface &opi,
                     PistacheServerInterface &serverInterface,
                     LoggedInAppUsersContainer &loggedInAppUsersContainer);

    // PistacheHandlerInterface interface
public:
    void method(CurrentContext &context);
};

#endif // HANDLERIBANCHECK_H
