#ifndef DATABASELOGIC_H
#define DATABASELOGIC_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "rapidjson/document.h"
#include "logstat/logstatcontroller.h"


class DatabaseLogic
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    void loginSuccessful(const std::string &loginEMail,
                         std::string &loginToken);

    const std::string t0001_users = {"t0001_users"};
    const std::string t0002_apps = {"t0002_apps"};
public:
    DatabaseLogic(LogStatController &logStatController,
                  PGConnectionPool &pool);

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

    bool saveApp(const sole::uuid owner_id,
                 const std::string &app_id,
                 const std::string &app_name,
                 const std::string &app_version,
                 const std::string &app_logo_url,
                 const std::string &app_color_name,
                 const std::string &json_yacapp,
                 const std::string &yacpck_base64,
                 std::string &message);

    size_t fetchAllAPPs(rapidjson::Document &target);
    bool fetchOneApp(const std::string &app_id,
                     rapidjson::Document &target);
};

#endif // DATABASELOGIC_H
