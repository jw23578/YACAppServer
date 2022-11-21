#ifndef HANDLERAPPUSERLOGIN_H
#define HANDLERAPPUSERLOGIN_H

#include <pistachehandlerinterface.h>
#include "databaselogicappuser.h"

class HandlerAppUserLogin : public PistacheHandlerInterface
{
    DatabaseLogicAppUser &databaseLogicAppUser;
public:
    HandlerAppUserLogin(DatabaseLogicAppUser &databaseLogicAppUser,
                        PistacheServerInterface &serverInterface);


    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERAPPUSERLOGIN_H
