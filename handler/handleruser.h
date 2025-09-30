#ifndef HANDLERUSER_H
#define HANDLERUSER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogics.h"
#include "caches/devicetokencache.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "emaillogic.h"

class HandlerUser : public PistacheHandlerInterface
{
    LoggedInAppUsersContainer &loggedInUsersContainer;
    EMailLogic &emailLogic;
    DatabaseLogics &databaseLogics;
    DeviceTokenCache &deviceTokenCache;

    bool thirdLogin(CurrentContext &context,
                    const std::string &third,
                    const std::string &mandant,
                    const std::string &loginEMail,
                    const std::string &password,
                    std::string &message,
                    ExtRapidJSONWriter &w,
                    reducedsole::uuid &appUserId);

public:
    HandlerUser(LoggedInAppUsersContainer &loggedInUsersContainer,
                EMailLogic &emailLogic,
                DatabaseLogics &databaseLogics,
                 DeviceTokenCache &deviceTokenCache,
                 PistacheServerInterface &serverInterface);


    // PistacheHandlerInterface interface
public:
    void method(CurrentContext &context) override;
};

#endif // HANDLERUSER_H
