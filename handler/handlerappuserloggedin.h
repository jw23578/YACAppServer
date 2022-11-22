#ifndef HANDLERAPPUSERLOGGEDIN_H
#define HANDLERAPPUSERLOGGEDIN_H

#include <pistachehandlerinterface.h>
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerAppUserLoggedIn : public PistacheHandlerInterface
{
    LoggedInAppUsersContainer &loggedInAppUsersContainer;
public:
    HandlerAppUserLoggedIn(LoggedInAppUsersContainer &loggedInAppUsersContainer,
                           PistacheServerInterface &serverInterface);


       // PistacheHandlerInterface interface
   public:
       void method() override;
};

#endif // HANDLERAPPUSERLOGGEDIN_H
