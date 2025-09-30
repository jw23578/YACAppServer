#ifndef HANDLERAPPUSERSEARCHPROFILES_H
#define HANDLERAPPUSERSEARCHPROFILES_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "databaselogicappuser.h"

class HandlerAppUserSearchProfiles : public HandlerLoggedInInterface
{
    DatabaseLogicAppUser &databaseLogicAppUser;
public:
    HandlerAppUserSearchProfiles(PistacheServerInterface &serverInterface,
                                 ORMPersistenceInterface &opi,
                                 DatabaseLogicAppUser &databaseLogicAppUser,
                                 LoggedInAppUsersContainer &loggedInAppUsersContainer);
    void method(CurrentContext &context) override;

};

#endif // HANDLERAPPUSERSEARCHPROFILES_H
