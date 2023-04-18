#ifndef DATABASELOGICUSERANDAPP_H
#define DATABASELOGICUSERANDAPP_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "rapidjson/document.h"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "tablefields.h"
#include "orm_implementions/t0002_apps.h"
#include "orm_implementions/t0027_app_images.h"


class DatabaseLogicUserAndApp
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    const TableNames tableNames;
    const TableFields tableFields;
    void loginSuccessful(const std::string &loginEMail,
                         std::string &loginToken);

    std::map<sole::uuid, pqxx::oid> imageId2oid;
    bool lookUpOid(const sole::uuid &imageId,
                   pqxx::oid &imageOid);

public:
    DatabaseLogicUserAndApp(LogStatController &logStatController,
                  PGConnectionPool &pool);

    bool userExists(const std::string &loginEMail);
    bool userIsAppOwner(const sole::uuid &app_id,
                        const sole::uuid &user_id,
                        std::string &errorMessage,
                        bool &appExists);
    std::string createUser(const std::string &loginEMail,
                           const std::string &password);

    bool verifyUser(const std::string &loginEMail,
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

    bool saveApp(const sole::uuid loggedInUserId,
                 t0002_apps &app,
                 const std::string &installation_code,
                 std::string &message);

    size_t getAllAPPs(rapidjson::Document &target);
    bool fetchOneApp(const std::string &app_id,
                     const int current_installed_version,
                     const std::string &installation_code,
                     rapidjson::Document &target);

    bool storeAppImage(t0027_app_images &t0027);

    bool getAppImage(const sole::uuid &id,
                     std::basic_string<std::byte> &data,
                     std::string &errorMessage);

};

#endif // DATABASELOGICUSERANDAPP_H
