#ifndef DATABASELOGICSPACES_H
#define DATABASELOGICSPACES_H

#include "postgres/pgconnectionpool.h"
#include "JWUtils/reducedsole.h"
#include "logstatcontroller.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"
#include "JWUtils/definitions.h"

class DatabaseLogicSpaces
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    TableNames tableNames;
    TableFields tableFields;
    bool fetchOneSpaceOnly(const reducedsole::uuid &id,
                           rapidjson::Value &object,
                           rapidjson::MemoryPoolAllocator<> &alloc,
                           std::string &message);
public:
    DatabaseLogicSpaces(LogStatController &logStatController,
                        PGConnectionPool &pool);

    bool insertOrUpdateSpace(reducedsole::uuid &id,
                             const reducedsole::uuid &app_id,
                             const std::string &name,
                             const reducedsole::uuid &creater_id,
                             const bool automatic,
                             const std::string &access_code,
                             const bool request_allowed,
                             rapidjson::Value &object,
                             rapidjson::MemoryPoolAllocator<> &alloc,
                             std::string &message);

    bool spaceRequestResultSeen(const reducedsole::uuid &id,
                                const reducedsole::uuid &appuser_id,
                                std::string &errorMessage);

    bool deleteSpace(const reducedsole::uuid &id,
                     const reducedsole::uuid &appuser_id,
                     std::string &message);

    bool fetchSpaces(const reducedsole::uuid &app_id, const reducedsole::uuid &appuser_id,
                     rapidjson::Value &targetArray,
                     rapidjson::MemoryPoolAllocator<> &alloc,
                     std::string &message);

    bool fetchSpace(const reducedsole::uuid &space_id,
                    rapidjson::Value &object,
                    rapidjson::MemoryPoolAllocator<> &alloc,
                    std::string &message);

    bool insertOrUpdateSpace2AppUser(reducedsole::uuid &id,
                                     const reducedsole::uuid &app_id,
                                     const reducedsole::uuid &space_id,
                                     const reducedsole::uuid &appuser_id,
                                     const TimePoint &requested_datetime,
                                     const TimePoint &approved_datetime,
                                     const reducedsole::uuid &approved_appuser_id,
                                     const TimePoint &denied_datetime,
                                     const reducedsole::uuid &denied_appuser_id);

    bool fetchSpaceRequests(const reducedsole::uuid &spaceAdminId,
                            rapidjson::Value &targetArray,
                            rapidjson::MemoryPoolAllocator<> &alloc,
                            std::string &errorMessage);

    bool fetchSpaceRequestId(const reducedsole::uuid &space_id,
                             const reducedsole::uuid &appuser_id,
                             reducedsole::uuid &id);

};

#endif // DATABASELOGICSPACES_H
