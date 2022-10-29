#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "interfaces/pistacheserverinterface.h"
#include "handler/handlerregisteruser.h"
#include "handler/handlerverifyuser.h"
#include "handler/handlerloginuser.h"
#include "handler/handleruserloggedin.h"
#include "handler/handlergetallapps.h"
#include "handlerLoggedIn/handleruploadapp.h"
#include "databaselogic.h"
#include "emaillogic.h"
#include "loggedinuserscontainer.h"

class YACAppServer: public PistacheServerInterface
{
    DatabaseLogic &databaseLogic;
    EMailLogic &emailLogic;
    LoggedInUsersContainer loggedInUsersContainer;
    HandlerRegisterUser handlerRegister;
    HandlerVerifyUser handlerVerifyUser;
    HandlerLoginUser handlerLoginUser;
    HandlerUserLoggedIn handlerUserLoggedIn;
    HandlerGetAllApps handlerGetAllApps;
    HandlerUploadApp handlerUploadApp;


public:
    YACAppServer(DatabaseLogic &databaseLogic,
                 EMailLogic &emailLogic,
                 int port);
};

#endif // YACAPPSERVER_H
