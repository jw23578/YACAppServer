#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "interfaces/pistacheserverinterface.h"
#include "handler/handleruserregister.h"
#include "handler/handlerverifyuser.h"
#include "handler/handleruserlogin.h"
#include "handler/handleruserloggedin.h"
#include "handler/handlergetallapps.h"
#include "handler/handlergetapp.h"
#include "handler/handlerappuserregister.h"
#include "handler/handlerappuserverify.h"
#include "handler/handlerappuserlogin.h"
#include "handler/handlerappuserloggedin.h"
#include "handlerLoggedIn/handleruploadapp.h"
#include "handlerLoggedIn/handlerstoremessage.h"
#include "databaselogicuserandapp.h"
#include "databaselogictables.h"
#include "databaselogicmessages.h"
#include "emaillogic.h"
#include "loggedincontainer/loggedinuserscontainer.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class YACAppServer: public PistacheServerInterface
{
    LoggedInUsersContainer loggedInUsersContainer;
    LoggedInAppUsersContainer loggedInAppUsersContainer;
    HandlerUserRegister handlerUserRegister;
    HandlerVerifyUser handlerVerifyUser;
    HandlerUserLogin handlerUserLogin;
    HandlerUserLoggedIn handlerUserLoggedIn;
    HandlerGetAllApps handlerGetAllApps;
    HandlerGetAPP handlerGetAPP;
    HandlerUploadApp handlerUploadApp;    

    HandlerAppUserRegister handlerAppUserRegister;
    HandlerAppUserVerify handlerAppUserVerify;
    HandlerAppUserLogin handlerAppUserLogin;
    HandlerAppUserLoggedIn handlerAppUserLoggedIn;

    HandlerStoreMessage handlerStoreMesage;


public:
    YACAppServer(DatabaseLogicTables &databaseLogicTables,
                 DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                 DatabaseLogicAppUser &databaseLogicAppUser,
                 DatabaseLogicMessages &databaseLogicMessages,
                 EMailLogic &emailLogic,
                 int port);
};

#endif // YACAPPSERVER_H
