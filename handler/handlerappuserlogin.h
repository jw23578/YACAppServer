#ifndef HANDLERAPPUSERLOGIN_H
#define HANDLERAPPUSERLOGIN_H

#include <pistachehandlerinterface.h>
#include "databaselogics.h"
#include "caches/devicetokencache.h"

class HandlerAppUserLogin : public PistacheHandlerInterface
{
    DatabaseLogics &databaseLogics;
    DeviceTokenCache &deviceTokenCache;

public:
    HandlerAppUserLogin(DatabaseLogics &databaseLogics,
                        DeviceTokenCache &deviceTokenCache,
                        PistacheServerInterface &serverInterface);


    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERAPPUSERLOGIN_H
