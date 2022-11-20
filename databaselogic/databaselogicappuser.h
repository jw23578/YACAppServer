#ifndef DATABASELOGICAPPUSER_H
#define DATABASELOGICAPPUSER_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "rapidjson/document.h"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"




class DatabaseLogicAppUser
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;
public:
    DatabaseLogicAppUser(LogStatController &logStatController,
                         PGConnectionPool &pool);

    bool appUserExists(const std::string &loginEMail);
    std::string createAppUser(const std::string &loginEMail,
                              const std::string &password);
    bool verifyAppUser(const std::string &loginEMail,
                       const std::string &verifyToken,
                       std::string &message,
                       std::string &loginToken);
    bool loginAppUser(const std::string &loginEMail,
                      const std::string &password,
                      std::string &message,
                      std::string &loginToken);

    bool appUserLoggedIn(const std::string &loginEMail,
                         const std::string &loginToken,
                         sole::uuid &userId,
                         std::chrono::system_clock::time_point &loginTokenValidUntil);

    void refreshAppUserLoginToken(const std::string &loginEMail,
                                  std::chrono::system_clock::time_point &loginTokenValidUntil);

};

#endif // DATABASELOGICAPPUSER_H
