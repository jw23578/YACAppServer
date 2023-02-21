#ifndef HANDLERAPPUSERUPDATEPROFILE_H
#define HANDLERAPPUSERUPDATEPROFILE_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "databaselogics.h"
#include "caches/devicetokencache.h"

class HandlerAppUserUpdateProfile : public HandlerLoggedInInterface
{
    DatabaseLogics &databaseLogics;
    DeviceTokenCache &deviceTokenCache;
public:
    HandlerAppUserUpdateProfile(PistacheServerInterface &serverInterface,
                                DatabaseLogics &databaseLogics,
                                DeviceTokenCache &deviceTokenCache,
                                LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method() override;
};

#endif // HANDLERAPPUSERUPDATEPROFILE_H
