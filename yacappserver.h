#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "interfaces/pistacheserverinterface.h"
#include "handler/handleruserregister.h"
#include "handler/handlerverifyuser.h"
#include "handler/handlerloginuser.h"
#include "handler/handleruserloggedin.h"
#include "handler/handlergetallapps.h"
#include "handler/handlergetapp.h"
#include "handler/handlerappuserregister.h"
#include "handler/handlerappuserverify.h"
#include "handlerLoggedIn/handleruploadapp.h"
#include "databaselogicuserandapp.h"
#include "databaselogictables.h"
#include "emaillogic.h"
#include "loggedinuserscontainer.h"

class YACAppServer: public PistacheServerInterface
{
    LoggedInUsersContainer loggedInUsersContainer;
    HandlerUserRegister handlerUserRegister;
    HandlerVerifyUser handlerVerifyUser;
    HandlerLoginUser handlerLoginUser;
    HandlerUserLoggedIn handlerUserLoggedIn;
    HandlerGetAllApps handlerGetAllApps;
    HandlerGetAPP handlerGetAPP;
    HandlerUploadApp handlerUploadApp;

    HandlerAppUserRegister handlerAppUserRegister;
    HandlerAppUserVerify handlerAppUserVerify;


public:
    YACAppServer(DatabaseLogicTables &databaseLogicTables,
                 DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                 DatabaseLogicAppUser &databaseLogicAppUser,
                 EMailLogic &emailLogic,
                 int port);
};

#endif // YACAPPSERVER_H
