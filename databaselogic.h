#ifndef DATABASELOGIC_H
#define DATABASELOGIC_H

#include "postgres/pgconnectionpool.h"

class DatabaseLogic
{
    PGConnectionPool &pool;
public:
    DatabaseLogic(PGConnectionPool &pool);

    bool connectionOk();
    bool pgCryptoInstalled();
    void createDatabaseTables();
    bool userExists(const std::string &loginEMail);
    std::string createUser(const std::string &loginEMail,
                           const std::string &password);
};

#endif // DATABASELOGIC_H
