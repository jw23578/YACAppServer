#include "yacappserver.h"

YACAppServer::YACAppServer(DatabaseLogic &databaseLogic,
                           EMailLogic &emailLogic,
                           int port):
    PistacheServerInterface(port),
    databaseLogic(databaseLogic),
    emailLogic(emailLogic),
    handlerRegister(databaseLogic,
                    emailLogic,
                    *this),
    handlerVerifyUser(databaseLogic,
                      *this),
    handlerLoginUser(databaseLogic,
                     *this)

{
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
