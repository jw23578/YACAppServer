#include "yacappserver.h"
#include "serverHeader/loginemailheader.h"
#include "serverHeader/logintokenheader.h"

YACAppServer::YACAppServer(DatabaseLogicTables &databaseLogicTables,
                           DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                           DatabaseLogicAppUser &databaseLogicAppUser,
                           EMailLogic &emailLogic,
                           int port):
    PistacheServerInterface(port),
    loggedInUsersContainer(databaseLogicUserAndApp),
    handlerUserRegister(databaseLogicUserAndApp,
                    emailLogic,
                    *this),
    handlerVerifyUser(databaseLogicUserAndApp,
                      *this),
    handlerLoginUser(databaseLogicUserAndApp,
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
                         *this)

{
    Pistache::Http::Header::Registrar<LoginEMailHeader>();
    Pistache::Http::Header::Registrar<LoginTokenHeader>();

    std::cout << "Checking Databaseconnection\n";
    if (!databaseLogicTables.connectionOk())
    {
        std::cout << "Databaseconnection is not ok\n";
        std::cout << "exiting\n";
        return;
    }
    std::cout << "Databaseconnection is ok\n";
    std::cout << "Checking for PGCrypto installed\n";
    if (!databaseLogicTables.pgCryptoInstalled())
    {
        std::cout << "PGCrypto is not installed\n";
        std::cout << "exiting\n";
        return;
    }
    std::cout << "PGCrypto is installed\n";
    databaseLogicTables.createDatabaseTables();
    std::cout << "Start Serving on Port: " << port << "\n";
    serve();
}
