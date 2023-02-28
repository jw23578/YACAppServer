#ifndef HANDLERAPPUSERVERIFY_H
#define HANDLERAPPUSERVERIFY_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogics.h""

class HandlerAppUserVerify : public PistacheHandlerInterface
{
    DatabaseLogics &databaseLogics;
public:
    HandlerAppUserVerify(DatabaseLogics &databaseLogics,
                         PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERAPPUSERVERIFY_H
