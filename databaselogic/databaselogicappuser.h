#ifndef DATABASELOGICAPPUSER_H
#define DATABASELOGICAPPUSER_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "rapidjson/document.h"


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

    std::map<std::string, sole::uuid> loginEMailAndAppId2AppUserId;
    bool lookupAppUser(const sole::uuid &appId,
                       const std::string &loginEMail,
                       sole::uuid &appUserId,
                       std::string &message);
    void refreshAppUserLoginToken(const sole::uuid &appId,
                                  const std::string &loginEMail,
                                  std::chrono::system_clock::time_point &loginTokenValidUntil);
    void resetUpdatePasswordToken(const sole::uuid &userId);
public:
    DatabaseLogicAppUser(LogStatController &logStatController,
                         PGConnectionPool &pool);

    bool appUserExists(const sole::uuid &appId,
                       const std::string &loginEMail);
    bool createAppUser(const sole::uuid &appId,
                       const std::string &loginEMail,
                       const std::string &password,
                       std::string &message,
                       std::string &verifyToken);
    bool verifyAppUser(const sole::uuid &appId,
                       const std::string &loginEMail,
                       const std::string &verifyToken,
                       std::string &message,
                       std::string &loginToken);
    bool loginAppUser(const sole::uuid &appId,
                      const std::string &loginEMail,
                      const std::string &password,
                      std::string &message,
                      std::string &loginToken);

    bool updateAppUser(const sole::uuid &appId,
                       const sole::uuid &userId,
                       const std::string &fstname,
                       const std::string &surname,
                       const std::string &visible_name,
                       const bool searching_exactly_allowed,
                       const bool searching_fuzzy_allowed,
                       std::string &message);

    bool appUserLoggedIn(const sole::uuid &appId,
                         const std::string &loginEMail,
                         const std::string &loginToken,
                         sole::uuid &userId,
                         std::chrono::system_clock::time_point &loginTokenValidUntil);

    bool requestUpdatePassword(const sole::uuid &appId,
                            const std::string &loginEMail,
                            std::string &updatePasswordToken,
                            std::string &message);

    bool updatePassword(const sole::uuid &appId,
                        const std::string &loginEMail,
                        const std::string &updatePasswordToken,
                        const std::string &password,
                        std::string &message,
                        std::string &loginToken,
                        sole::uuid &userId);

    bool deleteAppUser(const sole::uuid &appId,
                       const std::string &loginEMail,
                       const std::string &loginToken,
                       std::string &message);

    bool searchProfiles(const sole::uuid &appId,
                        const std::string &needle,
                        std::string &message,
                        rapidjson::Value &target,
                        rapidjson::MemoryPoolAllocator<> &alloc);

};

#endif // DATABASELOGICAPPUSER_H
