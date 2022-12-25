#include "yacappserver.h"
#include "serverHeader/loginemailheader.h"
#include "serverHeader/logintokenheader.h"
#include "serverHeader/appidheader.h"

YACAppServer::YACAppServer(DatabaseLogics &databaseLogics,
                           DatabaseLogicTables &databaseLogicTables,
                           DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                           DatabaseLogicAppUser &databaseLogicAppUser,
                           DatabaseLogicMessages &databaseLogicMessages,
                           EMailLogic &emailLogic,
                           int port):
    PistacheServerInterface(port),
    loggedInUsersContainer(databaseLogicUserAndApp),
    loggedInAppUsersContainer(databaseLogicAppUser),
    handlerUserRegister(databaseLogicUserAndApp,
                    emailLogic,
                    *this),
    handlerVerifyUser(databaseLogicUserAndApp,
                      *this),
    handlerUserLogin(databaseLogicUserAndApp,
                     *this),
    handlerUserLoggedIn(*this,
                        loggedInUsersContainer),
    handlerGetAllApps(databaseLogicUserAndApp,
                      *this),
    handlerGetAPP(databaseLogicUserAndApp,
                  *this),
    handlerUploadApp(databaseLogicUserAndApp,
                     *this,
                     loggedInUsersContainer),
    handlerAppUserRegister(databaseLogicAppUser,
                           emailLogic,
                           *this),
    handlerAppUserVerify(databaseLogicAppUser,
                         *this),
    handlerAppUserLogin(databaseLogicAppUser,
                        *this),
    handlerAppUserLoggedIn(loggedInAppUsersContainer,
                           *this),
    handlerAppUserRequestUpdatePassword(databaseLogicAppUser,
                                        emailLogic,
                                        *this),
    handlerAppUserUpdatePassword(databaseLogicAppUser,
                                 loggedInAppUsersContainer,
                                 *this),
    handlerAppUserUpdateProfile(*this,
                                databaseLogicAppUser,
                                loggedInAppUsersContainer),
    handlerAppUserSearchProfiles(*this,
                                 databaseLogicAppUser,
                                 loggedInAppUsersContainer),
    handlerAppUserGetWorktimeState(databaseLogics,
                                   *this,
                                   loggedInAppUsersContainer),
    handlerAppUserInsertWorktime(databaseLogics,
                                 *this,
                                 loggedInAppUsersContainer),
    handlerAppUserFetchProfile(databaseLogics,
                               *this,
                               loggedInAppUsersContainer),
    handlerStoreMesage(databaseLogicMessages,
                       *this,
                       loggedInAppUsersContainer)

{
    Pistache::Http::Header::Registrar<LoginEMailHeader>();
    Pistache::Http::Header::Registrar<LoginTokenHeader>();
    Pistache::Http::Header::Registrar<AppIdHeader>();

    std::cout << "Start Serving on Port: " << port << "\n";
    serve();
}
