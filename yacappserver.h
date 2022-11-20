#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "interfaces/pistacheserverinterface.h"
#include "handler/handlerregisteruser.h"
#include "handler/handlerverifyuser.h"
#include "handler/handlerloginuser.h"
#include "handler/handleruserloggedin.h"
#include "handler/handlergetallapps.h"
#include "handler/handlergetapp.h"
#include "handlerLoggedIn/handleruploadapp.h"
#include "databaselogicuserandapp.h"
#include "databaselogictables.h"
#include "emaillogic.h"
#include "loggedinuserscontainer.h"

class YACAppServer: public PistacheServerInterface
{
    DatabaseLogicTables &databaseLogicTables;
    DatabaseLogicUserAndApp &databaseLogicUserAndApp;
    EMailLogic &emailLogic;
    LoggedInUsersContainer loggedInUsersContainer;
    HandlerRegisterUser handlerRegister;
    HandlerVerifyUser handlerVerifyUser;
    HandlerLoginUser handlerLoginUser;
    HandlerUserLoggedIn handlerUserLoggedIn;
    HandlerGetAllApps handlerGetAllApps;
    HandlerGetAPP handlerGetAPP;
    HandlerUploadApp handlerUploadApp;


public:
    YACAppServer(DatabaseLogicTables &databaseLogicTables,
                 DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                 EMailLogic &emailLogic,
                 int port);
};

#endif // YACAPPSERVER_H
