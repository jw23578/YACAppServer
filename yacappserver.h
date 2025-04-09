#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "interfaces/pistacheserverinterface.h"
#include "handler/handleruserregister.h"
#include "handler/handlerverifyuser.h"
#include "handler/handleruserlogin.h"
#include "handler/handleruserloggedin.h"
#include "handler/handlerapp.h"
#include "handler/handlerpages.h"
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
#include "handlerLoggedIn/handlerappusergetworktimestate.h"
#include "handlerLoggedIn/handlerappuserinsertworktime.h"
#include "handlerLoggedIn/handlerappuserfetchprofile.h"
#include "handlerLoggedIn/handlerappuserfetchmessageupdates.h"
#include "handlerLoggedIn/handlerappuserfetchimage.h"
#include "handlerLoggedIn/handlerappuserrightgroup.h"
#include "handlerLoggedIn/handlerappuserspace.h"
#include "handlerLoggedIn/handlerormobjects.h"
#include "handlerLoggedIn/handler_t0030_documents.h"
#include "databaselogicuserandapp.h"
#include "databaselogics.h"
#include "emaillogic.h"
#include "loggedincontainer/loggedinuserscontainer.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "caches/devicetokencache.h"
#include "orm_implementions/yacormfactory.h"
#include "postgres/pgormpersistence.h"

class YACAppServer: public PistacheServerInterface
{
    DeviceTokenCache deviceTokenCache;

    LoggedInUsersContainer loggedInUsersContainer;
    LoggedInAppUsersContainer loggedInAppUsersContainer;
    HandlerUserRegister handlerUserRegister;
    HandlerVerifyUser handlerVerifyUser;
    HandlerUserLogin handlerUserLogin;
    HandlerUserLoggedIn handlerUserLoggedIn;
    HandlerAPP handlerAPP;
    HandlerPages handlerPages;
    HandlerUploadApp handlerUploadApp;    

    HandlerAppUserRegister handlerAppUserRegister;
    HandlerAppUserVerify handlerAppUserVerify;
    HandlerAppUserLogin handlerAppUserLogin;
    HandlerAppUserLoggedIn handlerAppUserLoggedIn;
    HandlerAppUserRequestUpdatePassword handlerAppUserRequestUpdatePassword;
    HandlerAppUserUpdatePassword handlerAppUserUpdatePassword;
    HandlerAppUserUpdateProfile handlerAppUserUpdateProfile;
    HandlerAppUserSearchProfiles handlerAppUserSearchProfiles;
    HandlerAppUserGetWorktimeState handlerAppUserGetWorktimeState;
    HandlerAppUserInsertWorktime handlerAppUserInsertWorktime;
    HandlerAppUserFetchProfile handlerAppUserFetchProfile;
    HandlerAppUserFetchMessageUpdates handlerAppUserFetchMessageUpdates;
    HandlerAppUserFetchImage handlerAppUserFetchImage;
    Handler_t0030_documents handler_t0030_documents;

    HandlerAppUserRightGroup handlerAppUserRightGroup;
    HandlerAppUserSpace handlerAppUserSpace;
    HandlerORMObjects handlerORMObjects;
    HandlerStoreMessage handlerStoreMesage;


public:
    YACAppServer(const std::string &firebaseApiKey,
                 YACORMFactory &factory,
                 PGORMPersistence &opi,
                 DatabaseLogics &databaseLogics,
                 DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                 DatabaseLogicAppUser &databaseLogicAppUser,
                 EMailLogic &emailLogic,
                 int port,
                 const std::string &certFilename,
                 const std::string &keyFilename);
};

#endif // YACAPPSERVER_H
