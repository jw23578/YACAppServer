#ifndef DATABASELOGICSPACES_H
#define DATABASELOGICSPACES_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"
#include "definitions.h"

class DatabaseLogicSpaces
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    TableNames tableNames;
    TableFields tableFields;
    bool fetchOneSpaceOnly(const sole::uuid &id,
                           rapidjson::Value &object,
                           rapidjson::MemoryPoolAllocator<> &alloc,
                           std::string &message);
public:
    DatabaseLogicSpaces(LogStatController &logStatController,
                        PGConnectionPool &pool);

    bool insertOrUpdateSpace(sole::uuid &id,
                             const sole::uuid &app_id,
                             const std::string &name,
                             const sole::uuid &creater_id,
                             const bool automatic,
                             const std::string &access_code,
                             rapidjson::Value &object,
                             rapidjson::MemoryPoolAllocator<> &alloc,
                             std::string &message);

    bool deleteSpace(const sole::uuid &id,
                     const sole::uuid &appuser_id,
                     std::string &message);

    bool fetchSpaces(const sole::uuid &app_id, const sole::uuid &appuser_id,
                     rapidjson::Value &targetArray,
                     rapidjson::MemoryPoolAllocator<> &alloc,
                     std::string &message);

    bool fetchSpace(const sole::uuid &space_id,
                    rapidjson::Value &object,
                    rapidjson::MemoryPoolAllocator<> &alloc,
                    std::string &message);

    bool insertOrUpdateSpace2AppUser(sole::uuid &id,
                                     const sole::uuid &app_id,
                                     const sole::uuid &space_id,
                                     const sole::uuid &appuser_id,
                                     const TimePoint &requested_datetime,
                                     const TimePoint &approved_datetime,
                                     const sole::uuid &approved_appuser_id,
                                     const TimePoint &denied_datetime,
                                     const sole::uuid &denied_appuser_id);

    bool fetchSpaceRequests(const sole::uuid &app_id,
                            rapidjson::Value &targetArray,
                            rapidjson::MemoryPoolAllocator<> &alloc,
                            std::string &message);

};

#endif // DATABASELOGICSPACES_H
