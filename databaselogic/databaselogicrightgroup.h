#ifndef DATABASELOGICRIGHTGROUP_H
#define DATABASELOGICRIGHTGROUP_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"
#include <set>
#include "definitions.h"

class DatabaseLogicRightGroup
{
    LogStatController &logStatController;
    TableNames tableNames;
    TableFields tableFields;

    bool fetchOneRightGroup(const sole::uuid &id,
                            rapidjson::Value &object,
                            rapidjson::MemoryPoolAllocator<> &alloc,
                            std::string &message);

public:
    bool appuserInRightGroup(const sole::uuid &right_group_id,
                             const sole::uuid &appuser_id);
    PGConnectionPool &pool;
    DatabaseLogicRightGroup(LogStatController &logStatController,
                            PGConnectionPool &pool);

    bool fetchIDOfOneRightGroupByName(const sole::uuid &app_id,
                                      const std::string &name,
                                      sole::uuid &id);

    bool deleteRightGroup(const sole::uuid &id,
                          const sole::uuid &appuser_id,
                          std::string &message);

    bool fetchRightGroup(const sole::uuid &right_group_id,
                         rapidjson::Value &object,
                         rapidjson::MemoryPoolAllocator<> &alloc,
                         std::string &message);

    bool fetchRightGroupMember(const sole::uuid &right_group_id,
                               rapidjson::Value &member,
                               rapidjson::MemoryPoolAllocator<> &alloc,
                               std::string &errorMessage);

    void fetchGroupRightNumbers(const sole::uuid &right_group_id,
                                std::set<int> &right_numbers);

    void fetchAppUserRightNumbers(const sole::uuid &appuser_id,
                                  std::set<int> &right_numbers);

    bool insertRight(const sole::uuid &id,
                     const sole::uuid &right_group_id,
                     const int right_number,
                     std::string &message);

    bool removeRight(const sole::uuid &right_group_id,
                     const int right_number,
                     std::string &message);

    bool removeUser(const sole::uuid &right_group_id,
                    const sole::uuid &appuser_id,
                    std::string &message);

    void checkAndGenerateAdminGroup(const sole::uuid &app_id,
                                    const std::string &adminGroupName,
                                    const std::set<int> &right_numbers);
    bool adminExists(const sole::uuid &app_id,
                     const std::string &adminGroupName);
};

#endif // DATABASELOGICRIGHTGROUP_H
