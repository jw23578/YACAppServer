#ifndef HANDLERAPPUSERAPPOINTMENTS_H
#define HANDLERAPPUSERAPPOINTMENTS_H

#include "handlerloggedininterface.h"
#include "databaselogics.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerAppUserAppointments : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserAppointments(DatabaseLogics &databaseLogics,
                               PistacheServerInterface &serverInterface,
                               LoggedInAppUsersContainer &loggedInAppUsersContainer);


    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERAPPUSERAPPOINTMENTS_H
