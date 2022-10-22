#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "interfaces/pistacheserverinterface.h"
#include "handler/handlerregisteruser.h"
#include "handler/handlerverifyuser.h"
#include "handler/handlerloginuser.h"
#include "databaselogic.h"
#include "emaillogic.h"

class YACAppServer: public PistacheServerInterface
{
    DatabaseLogic &databaseLogic;
    EMailLogic &emailLogic;
    HandlerRegisterUser handlerRegister;
    HandlerVerifyUser handlerVerifyUser;
    HandlerLoginUser handlerLoginUser;

public:
    YACAppServer(DatabaseLogic &databaseLogic,
                 EMailLogic &emailLogic,
                 int port);
};

#endif // YACAPPSERVER_H
