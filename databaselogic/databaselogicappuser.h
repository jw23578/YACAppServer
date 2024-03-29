#ifndef DATABASELOGICAPPUSER_H
#define DATABASELOGICAPPUSER_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"
#include "extrapidjson.h"
#include "orm_implementions/t0002_apps.h"
#include "orm_implementions/t0003_appuser_profiles.h"


class LoggedInAppUsersContainer;

class DatabaseLogicAppUser
{
    friend LoggedInAppUsersContainer;

    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;
    TableFields tableFields;
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

    sole::uuid getAppUserId(const sole::uuid &appId,
                            const std::string &loginEMail);
    bool createAppUser(const sole::uuid &appId,
                       const std::string &loginEMail,
                       const std::string &password,
                       std::string &message,
                       std::string &verifyToken);

    bool createVerifiedAppUser(const sole::uuid &appId,
                               const std::string &loginEMail,
                               const std::string &fstname,
                               const std::string &surname,
                               const std::string &visible_name,
                               const bool searching_exactly_allowed,
                               const bool searching_fuzzy_allowed,
                               const std::string &public_key_base64,
                               std::string &message,
                               t0003_appuser_profiles &target);

    bool createVerifyToken(const sole::uuid &appId,
                           const std::string &loginEMail,
                           std::string &message,
                           std::string &verifyToken);

    bool verifyAppUser(const sole::uuid &appId,
                       const std::string &loginEMail,
                       const std::string &verifyToken,
                       std::string &message,
                       ExtRapidJSONWriter &w,
                       sole::uuid &appUserId);
    bool loginAppUser(const sole::uuid &appId,
                      const std::string &loginEMail,
                      const std::string &password,
                      std::string &message,
                      ExtRapidJSONWriter &w,
                      sole::uuid &appUserId);

    bool updateAppUser(const sole::uuid &appId,
                       const sole::uuid &userId,
                       const std::string &fstname,
                       const std::string &surname,
                       const std::string &visible_name,
                       const std::string &color,
                       const std::string &message_font_color,
                       const bool searching_exactly_allowed,
                       const bool searching_fuzzy_allowed,
                       const std::string &public_key_base64,
                       const bool with_image,
                       const sole::uuid imageId,
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

    bool updatePasswordLoggedIn(const sole::uuid &appuser_id,
                                const std::string &password);
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
                        size_t limit,
                        const size_t offset,
                        std::string &message,
                        rapidjson::Value &target,
                        rapidjson::MemoryPoolAllocator<> &alloc);

    bool fetchMyProfile(const sole::uuid &appId,
                        const sole::uuid &userId,
                        std::string &message,
                        rapidjson::Value &target,
                        rapidjson::MemoryPoolAllocator<> &alloc);

    bool fetchProfile(const sole::uuid &appId,
                      const sole::uuid &userId,
                      std::string &message,
                      rapidjson::Value &target,
                      rapidjson::MemoryPoolAllocator<> &alloc);

    bool storeDeviceToken(const sole::uuid &userId,
                          const std::string &device_token);

    bool removeDeviceToken(const sole::uuid &userId,
                           const std::string &device_token);

    size_t fetchDeviceToken(const sole::uuid &userId,
                            std::set<std::string> &device_token);
};

#endif // DATABASELOGICAPPUSER_H
