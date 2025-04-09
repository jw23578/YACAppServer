#ifndef HANDLERAPPUSERLOGIN_H
#define HANDLERAPPUSERLOGIN_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogics.h"
#include "caches/devicetokencache.h"

class HandlerAppUserLogin : public PistacheHandlerInterface
{
    DatabaseLogics &databaseLogics;
    DeviceTokenCache &deviceTokenCache;

    bool thirdLogin(const reducedsole::uuid &appId,
                    const std::string &third,
                    const std::string &mandant,
                    const std::string &loginEMail,
                    const std::string &password,
                    std::string &message,
                    ExtRapidJSONWriter &w,
                    reducedsole::uuid &appUserId);

public:
    HandlerAppUserLogin(DatabaseLogics &databaseLogics,
                        DeviceTokenCache &deviceTokenCache,
                        PistacheServerInterface &serverInterface);


    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERAPPUSERLOGIN_H
