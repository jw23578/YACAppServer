#include "yacappserver.h"
#include "pistache/router.h"
#include "postgres/pgconnection.h"
#include "postgres/pgutils.h"
#include "postgres/pgsqlstring.h"
#include "postgres/pgcommandtransactor.h"

void YACAppServer::createDatabaseTables()
{
    PGUtils utils(postgresConnectionPool);
    std::string t0001_users("t0001_users");
    if (!utils.tableExists(t0001_users))
    {
        std::cout << "creating table " << t0001_users << "\n";
        PGSqlString sql("create table ");
        sql += t0001_users;
        sql += std::string(" ( id uuid, "
                           " loginemail text, "
                           " password_hash text, "
                           " verified timestamp, "
                           " verify_token text, "
                           " login_token text, "
                           " login_token_valid_until timestamp) ");
        pqxx::result r;
        PGCommandTransactor ct(postgresConnectionPool, sql, r);
    }
}

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
    createDatabaseTables();
    std::cout << "Start Serving on Port: " << port << "\n";
    serve();
}
