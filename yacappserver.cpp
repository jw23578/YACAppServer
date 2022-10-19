#include "yacappserver.h"
#include "pistache/router.h"
#include "postgres/pgconnection.h"
#include "postgres/pgutils.h"
#include "postgres/pgsqlstring.h"
#include "postgres/pgcommandtransactor.h"

YACAppServer::YACAppServer(std::string const &postgresHost,
                           int postgresPort,
                           std::string const &postgresDBName,
                           std::string const &postgresUser,
                           std::string const &postgresPassword,
                           int port):
    PistacheServerInterface(port),
    postgresConnectionPool(postgresHost,
                           postgresPort,
                           postgresDBName,
                           postgresUser,
                           postgresPassword,
                           10),
    databaseLogic(postgresConnectionPool),
    handlerRegister(databaseLogic, *this)

{
    std::cout << "Checking Databaseconnection\n";
    try
    {
        PGConnection connection(postgresConnectionPool);
    }
    catch (...)
    {
        std::cout << "Databaseconnection is not ok\n";
        std::cout << "exiting\n";
        return;
    }
    std::cout << "Databaseconnection is ok\n";
    std::cout << "Checking for PGCrypto installed\n";
    PGUtils utils(postgresConnectionPool);
    if (!utils.pgCryptoInstalled())
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
