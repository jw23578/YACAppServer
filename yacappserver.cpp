#include "yacappserver.h"
#include "serverHeader/loginemailheader.h"
#include "serverHeader/logintokenheader.h"
#include "serverHeader/appidheader.h"
#include "serverHeader/thirdheader.h"
#include "serverHeader/mandantheader.h"

YACAppServer::YACAppServer(const std::string &firebaseApiKey,
                           YACORMFactory &factory,
                           PGORMPersistence &opi,
                           DatabaseLogics &databaseLogics,
                           DatabaseLogicAppUser &databaseLogicAppUser,
                           EMailLogic &emailLogic,
                           int port,
                           const std::string &certFilename,
                           const std::string &keyFilename):
    PistacheServerInterface(port,
                            certFilename,
                            keyFilename),
    deviceTokenCache(databaseLogics),
    loggedInAppUsersContainer(opi,
                              databaseLogics),
    handlerAPP(opi,
               *this),
    handlerPages(*this, opi),
    handlerUploadApp(opi,
                     *this,
                     loggedInAppUsersContainer),
    handlerUser(loggedInAppUsersContainer,
                emailLogic,
                databaseLogics,
                deviceTokenCache,
                *this),
    handlerAppUserUpdateProfile(*this,
                                databaseLogics,
                                deviceTokenCache,
                                loggedInAppUsersContainer),
    handlerAppUserSearchProfiles(*this,
                                   opi,
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
    handlerAppUserFetchMessageUpdates(*this,
                                      databaseLogics,
                                      loggedInAppUsersContainer),
    handlerAppUserFetchImage(*this,
                             databaseLogics,
                             loggedInAppUsersContainer),
    handler_t0030_documents(*this,
                            loggedInAppUsersContainer,
                            opi),
    handlerAppUserRightGroup(databaseLogics,
                             *this,
                             loggedInAppUsersContainer),
    handlerAppUserSpace(databaseLogics,
                        *this,
                        loggedInAppUsersContainer),
    handlerORMObjects(databaseLogics,
                      factory,
                      databaseLogics.pool,
                      databaseLogics.rightsLogic,
                      *this,
                      loggedInAppUsersContainer),
    handlerStoreMesage(firebaseApiKey,
                       deviceTokenCache,
                       databaseLogics,
                       *this,
                       loggedInAppUsersContainer)
{
    Pistache::Http::Header::Registrar<LoginEMailHeader>();
    Pistache::Http::Header::Registrar<LoginTokenHeader>();
    Pistache::Http::Header::Registrar<AppIdHeader>();
    Pistache::Http::Header::Registrar<ThirdHeader>();
    Pistache::Http::Header::Registrar<MandantHeader>();

    std::cout << "Start Serving on Port: " << port << "\n";
    serve();
}
