#ifndef HANDLERAPPUSERVERIFY_H
#define HANDLERAPPUSERVERIFY_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogicappuser.h"

class HandlerAppUserVerify : public PistacheHandlerInterface
{
    DatabaseLogicAppUser &databaseLogicAppUser;
public:
    HandlerAppUserVerify(DatabaseLogicAppUser &databaseLogicAppUser,
                         PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERAPPUSERVERIFY_H
