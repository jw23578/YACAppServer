#ifndef DATABASELOGICAPPUSER_H
#define DATABASELOGICAPPUSER_H

#include "pgconnectionpool.h"
#include "JWUtils/reducedsole.h"
#include "logstatcontroller.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"
#include "extrapidjson.h"
#include "orm_implementions/t0001_apps.h"
#include "orm_implementions/t0002_user.h"
#include "ormpersistenceinterface.h"


class LoggedInAppUsersContainer;

class DatabaseLogicAppUser
{
    friend LoggedInAppUsersContainer;

    LogStatController &logStatController;
    PGConnectionPool &pool;
    ORMPersistenceInterface &opi;
    TableNames tableNames;
    TableFields tableFields;
    void loginSuccessful(const reducedsole::uuid &appId,
                         const reducedsole::uuid &userId,
                         std::string &loginToken);

    void refreshAppUserLoginToken(CurrentContext &context,
                                  const std::string &loginEMail,
                                  std::chrono::system_clock::time_point &loginTokenValidUntil);

public:
    DatabaseLogicAppUser(LogStatController &logStatController,
                         PGConnectionPool &pool,
                         ORMPersistenceInterface &opi);

    bool loginUser(CurrentContext &context,
                   const std::string &loginEMail,
                   const std::string &password,
                   std::string &message,
                   ExtRapidJSONWriter &w,
                   reducedsole::uuid &appUserId);
    bool logoutAppUserByLoginToken(const std::string lt);

    bool appUserLoggedIn(CurrentContext &context,
                         const std::string &loginEMail,
                         const std::string &loginToken,
                         reducedsole::uuid &userId,
                         std::chrono::system_clock::time_point &loginTokenValidUntil);

    bool requestUpdatePassword(CurrentContext &context,
                               const std::string &loginEMail,
                               std::string &updatePasswordToken,
                               std::string &message);

    bool updatePasswordLoggedIn(const reducedsole::uuid &user_id,
                                const std::string &password);
    bool updatePassword(CurrentContext &context,
                        const std::string &loginEMail,
                        const std::string &updatePasswordToken,
                        const std::string &password,
                        std::string &message,
                        std::string &loginToken,
                        reducedsole::uuid &userId);

    bool deleteAppUser(const reducedsole::uuid &appId,
                       const std::string &loginEMail,
                       const std::string &loginToken,
                       std::string &message);

    bool searchProfiles(const reducedsole::uuid &appId,
                        const std::string &needle,
                        size_t limit,
                        const size_t offset,
                        std::string &message,
                        rapidjson::Value &target,
                        rapidjson::MemoryPoolAllocator<> &alloc);

    bool fetchMyProfile(const reducedsole::uuid &appId,
                        const reducedsole::uuid &userId,
                        std::string &message,
                        rapidjson::Value &target,
                        rapidjson::MemoryPoolAllocator<> &alloc);

    bool fetchProfile(const reducedsole::uuid &appId,
                      const reducedsole::uuid &userId,
                      std::string &message,
                      rapidjson::Value &target,
                      rapidjson::MemoryPoolAllocator<> &alloc);

    bool storeDeviceToken(const reducedsole::uuid &userId,
                          const std::string &device_token);

    bool removeDeviceToken(const reducedsole::uuid &userId,
                           const std::string &device_token);

    size_t fetchDeviceToken(const reducedsole::uuid &userId,
                            std::set<std::string> &device_token);
};

#endif // DATABASELOGICAPPUSER_H
