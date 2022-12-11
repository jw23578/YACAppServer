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
#include "handler/handlerappuserrequestupdatepassword.h"
#include "handler/handlerappuserupdatepassword.h"
#include "handlerLoggedIn/handleruploadapp.h"
#include "handlerLoggedIn/handlerstoremessage.h"
#include "handlerLoggedIn/handlerappuserupdateprofile.h"
#include "handlerLoggedIn/handlerappusersearchprofiles.h"
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
    HandlerAppUserRequestUpdatePassword handlerAppUserRequestUpdatePassword;
    HandlerAppUserUpdatePassword handlerAppUserUpdatePassword;
    HandlerAppUserUpdateProfile handlerAppUserUpdateProfile;
    HandlerAppUserSearchProfiles handlerAppUserSearchProfiles;

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
