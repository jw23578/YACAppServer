#ifndef HANDLERUSERLOGGEDIN_H
#define HANDLERUSERLOGGEDIN_H

#include "loggedincontainer/loggedinuserscontainer.h"
#include <pistachehandlerinterface.h>

class HandlerUserLoggedIn : public PistacheHandlerInterface
{
    LoggedInUsersContainer &loggedInUsersContainer;
public:
    HandlerUserLoggedIn(PistacheServerInterface &serverInterface,
                        LoggedInUsersContainer &loggedInUsersContainer);
    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERUSERLOGGEDIN_H
