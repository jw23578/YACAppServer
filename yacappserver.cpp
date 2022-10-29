#include "yacappserver.h"
#include "serverHeader/loginemailheader.h"
#include "serverHeader/logintokenheader.h"

YACAppServer::YACAppServer(DatabaseLogic &databaseLogic,
                           EMailLogic &emailLogic,
                           int port):
    PistacheServerInterface(port),
    databaseLogic(databaseLogic),
    emailLogic(emailLogic),
    loggedInUsersContainer(databaseLogic),
    handlerRegister(databaseLogic,
                    emailLogic,
                    *this),
    handlerVerifyUser(databaseLogic,
                      *this),
    handlerLoginUser(databaseLogic,
                     *this),
    handlerUserLoggedIn(*this,
                        loggedInUsersContainer),
    handlerGetAllApps(databaseLogic,
                      *this),
    handlerUploadApp(databaseLogic,
                     *this,
                     loggedInUsersContainer)

{
    Pistache::Http::Header::Registrar<LoginEMailHeader>();
    Pistache::Http::Header::Registrar<LoginTokenHeader>();

    std::cout << "Checking Databaseconnection\n";
    if (!databaseLogic.connectionOk())
    {
        std::cout << "Databaseconnection is not ok\n";
        std::cout << "exiting\n";
        return;
    }
    std::cout << "Databaseconnection is ok\n";
    std::cout << "Checking for PGCrypto installed\n";
    if (!databaseLogic.pgCryptoInstalled())
    {
        std::cout << "PGCrypto is not installed\n";
        std::cout << "exiting\n";
        return;
    }
    std::cout << "PGCrypto is installed\n";
    databaseLogic.createDatabaseTables();
    std::cout << "Start Serving on Port: " << port << "\n";
    serve();
}
