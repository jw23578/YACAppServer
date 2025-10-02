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
public:
    DatabaseLogicAppUser(LogStatController &logStatController,
                         PGConnectionPool &pool,
                         ORMPersistenceInterface &opi);

    bool searchProfiles(const reducedsole::uuid &appId,
                        const std::string &needle,
                        size_t limit,
                        const size_t offset,
                        std::string &message,
                        rapidjson::Value &target,
                        rapidjson::MemoryPoolAllocator<> &alloc);

};

#endif // DATABASELOGICAPPUSER_H
