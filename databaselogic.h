#ifndef DATABASELOGIC_H
#define DATABASELOGIC_H

#include "postgres/pgconnectionpool.h"

class DatabaseLogic
{
    PGConnectionPool &pool;
    void loginSuccessful(const std::string &loginEMail,
                         std::string &loginToken);
public:
    DatabaseLogic(PGConnectionPool &pool);

    bool connectionOk();
    bool pgCryptoInstalled();
    void createDatabaseTables();
    bool userExists(const std::string &loginEMail);
    std::string createUser(const std::string &loginEMail,
                           const std::string &password);

    bool verfiyUser(const std::string &loginEMail,
                    const std::string &verifyToken,
                    std::string &message,
                    std::string &loginToken);
};

#endif // DATABASELOGIC_H
