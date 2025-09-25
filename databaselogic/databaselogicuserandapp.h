#ifndef DATABASELOGICUSERANDAPP_H
#define DATABASELOGICUSERANDAPP_H

#include "pgconnectionpool.h"
#include "JWUtils/reducedsole.h"
#include "rapidjson/document.h"
#include "logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "tablefields.h"
#include "orm_implementions/t0001_apps.h"
#include "orm_implementions/t0027_app_images.h"
#include "ormpersistenceinterface.h"


class DatabaseLogicUserAndApp
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    ORMPersistenceInterface &opi;
    const TableNames tableNames;
    const TableFields tableFields;

    std::map<reducedsole::uuid, pqxx::oid> imageId2oid;
    bool lookUpOid(const reducedsole::uuid &imageId,
                   pqxx::oid &imageOid);

public:
    DatabaseLogicUserAndApp(LogStatController &logStatController,
                            PGConnectionPool &pool,
                            ORMPersistenceInterface &opi);

    bool userIsAppOwner(const reducedsole::uuid &app_id,
                        const reducedsole::uuid &user_id,
                        std::string &errorMessage,
                        bool &appExists);
    bool logoutByLoginToken(const std::string &loginToken);

    bool saveApp(const reducedsole::uuid loggedInUserId,
                 t0001_apps &app,
                 const std::string &installation_code,
                 std::string &message);

    size_t getAllAPPs(rapidjson::Document &target);
    bool fetchOneApp(const std::string &app_id,
                     const int current_installed_version,
                     const std::string &installation_code,
                     rapidjson::Document &target);

    bool storeAppImage(t0027_app_images &t0027);

    bool getAppImage(const reducedsole::uuid &id,
                     std::basic_string<std::byte> &data,
                     std::string &errorMessage);

};

#endif // DATABASELOGICUSERANDAPP_H
