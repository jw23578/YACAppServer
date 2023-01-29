#ifndef HANDLERSTOREMESSAGE_H
#define HANDLERSTOREMESSAGE_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "databaselogic/databaselogics.h"
#include "caches/devicetokencache.h"

class HandlerStoreMessage : public HandlerLoggedInInterface
{
    const std::string firebaseApiKey;
    DeviceTokenCache &deviceTokenCache;
    DatabaseLogics &databaseLogics;
public:
    HandlerStoreMessage(const std::string &firebaseKey,
                        DeviceTokenCache &deviceTokenCache,
                        DatabaseLogics &databaseLogics,
                        PistacheServerInterface &serverInterface,
                        LoggedInAppUsersContainer &loggedInAppUsersContainer);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERSTOREMESSAGE_H
