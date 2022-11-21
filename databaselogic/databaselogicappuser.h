#ifndef DATABASELOGICAPPUSER_H
#define DATABASELOGICAPPUSER_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"


class LoggedInAppUsersContainer;

class DatabaseLogicAppUser
{
    friend LoggedInAppUsersContainer;

    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;
    void loginSuccessful(const sole::uuid &appUserId,
                         std::string &loginToken);

    std::map<std::string, sole::uuid> loginEMail2AppUserId;
    bool lookupAppUser(const std::string &loginEMail,
                       sole::uuid &appUserId,
                       std::string &message);
    void refreshAppUserLoginToken(const std::string &loginEMail,
                                  std::chrono::system_clock::time_point &loginTokenValidUntil);
public:
    DatabaseLogicAppUser(LogStatController &logStatController,
                         PGConnectionPool &pool);

    bool appUserExists(const std::string &loginEMail);
    bool createAppUser(const sole::uuid &appId,
                       const std::string &loginEMail,
                       const std::string &password,
                       std::string &message,
                       std::string &verifyToken);
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


};

#endif // DATABASELOGICAPPUSER_H
