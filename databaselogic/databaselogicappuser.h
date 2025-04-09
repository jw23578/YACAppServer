#ifndef DATABASELOGICAPPUSER_H
#define DATABASELOGICAPPUSER_H

#include "pgconnectionpool.h"
#include "JWUtils/reducedsole.h"
#include "logstatcontroller.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"
#include "extrapidjson.h"
#include "orm_implementions/t0002_apps.h"
#include "orm_implementions/t0003_appuser_profiles.h"
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
    void loginSuccessful(const reducedsole::uuid &appUserId,
                         std::string &loginToken);

    std::map<std::string, reducedsole::uuid> loginEMailAndAppId2AppUserId;
    bool lookupAppUser(const reducedsole::uuid &appId,
                       const std::string &loginEMail,
                       reducedsole::uuid &appUserId,
                       std::string &message);
    void refreshAppUserLoginToken(const reducedsole::uuid &appId,
                                  const std::string &loginEMail,
                                  std::chrono::system_clock::time_point &loginTokenValidUntil);
    void resetUpdatePasswordToken(const reducedsole::uuid &userId);

public:
    DatabaseLogicAppUser(LogStatController &logStatController,
                         PGConnectionPool &pool,
                         ORMPersistenceInterface &opi);

    reducedsole::uuid getAppUserId(const reducedsole::uuid &appId,
                            const std::string &loginEMail);
    bool createAppUser(const reducedsole::uuid &appId,
                       const std::string &loginEMail,
                       const std::string &password,
                       std::string &message,
                       std::string &verifyToken);

    bool createVerifiedAppUser(const reducedsole::uuid &appId,
                               const std::string &loginEMail,
                               const std::string &fstname,
                               const std::string &surname,
                               const std::string &visible_name,
                               const bool searching_exactly_allowed,
                               const bool searching_fuzzy_allowed,
                               const std::string &public_key_base64,
                               std::string &message,
                               t0003_appuser_profiles &target);

    bool createVerifyToken(const reducedsole::uuid &appId,
                           const std::string &loginEMail,
                           std::string &message,
                           std::string &verifyToken);

    bool verifyAppUser(const reducedsole::uuid &appId,
                       const std::string &loginEMail,
                       const std::string &verifyToken,
                       std::string &message,
                       ExtRapidJSONWriter &w,
                       reducedsole::uuid &appUserId);
    bool loginAppUser(const reducedsole::uuid &appId,
                      const std::string &loginEMail,
                      const std::string &password,
                      std::string &message,
                      ExtRapidJSONWriter &w,
                      reducedsole::uuid &appUserId);

    bool updateAppUser(const reducedsole::uuid &appId,
                       const reducedsole::uuid &userId,
                       const std::string &fstname,
                       const std::string &surname,
                       const std::string &visible_name,
                       const std::string &color,
                       const std::string &message_font_color,
                       const bool searching_exactly_allowed,
                       const bool searching_fuzzy_allowed,
                       const std::string &public_key_base64,
                       const bool with_image,
                       const reducedsole::uuid imageId,
                       std::string &message);

    bool appUserLoggedIn(const reducedsole::uuid &appId,
                         const std::string &loginEMail,
                         const std::string &loginToken,
                         reducedsole::uuid &userId,
                         std::chrono::system_clock::time_point &loginTokenValidUntil);

    bool requestUpdatePassword(const reducedsole::uuid &appId,
                               const std::string &loginEMail,
                               std::string &updatePasswordToken,
                               std::string &message);

    bool updatePasswordLoggedIn(const reducedsole::uuid &appuser_id,
                                const std::string &password);
    bool updatePassword(const reducedsole::uuid &appId,
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
