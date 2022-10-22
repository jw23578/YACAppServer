#ifndef DATABASELOGIC_H
#define DATABASELOGIC_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"

class DatabaseLogic
{
    PGConnectionPool &pool;
    void loginSuccessful(const std::string &loginEMail,
                         std::string &loginToken);

    const std::string t0001_users = {"t0001_users"};
    const std::string t0002_apps = {"t0002_apps"};
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

    bool loginUser(const std::string &loginEMail,
                   const std::string &password,
                   std::string &message,
                   std::string &loginToken);

    bool userLoggedIn(const std::string &loginEMail,
                      const std::string &loginToken,
                      sole::uuid &userId,
                      std::chrono::system_clock::time_point &loginTokenValidUntil);

    void refreshLoginToken(const std::string &loginEMail,
                           std::chrono::system_clock::time_point &loginTokenValidUntil);

    void saveApp(const std::string &appId,
                   const std::string &json_yacapp,
                   const std::string &yacpck_base64);
};

#endif // DATABASELOGIC_H
