#ifndef DATABASELOGICRIGHTGROUP_H
#define DATABASELOGICRIGHTGROUP_H

#include "postgres/pgconnectionpool.h"
#include "utils/reducedsole.h"
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

    bool fetchOneRightGroup(const reducedsole::uuid &id,
                            rapidjson::Value &object,
                            rapidjson::MemoryPoolAllocator<> &alloc,
                            std::string &message);

public:
    bool appuserInRightGroup(const reducedsole::uuid &right_group_id,
                             const reducedsole::uuid &appuser_id);
    PGConnectionPool &pool;
    DatabaseLogicRightGroup(LogStatController &logStatController,
                            PGConnectionPool &pool);

    bool fetchIDOfOneRightGroupByName(const reducedsole::uuid &app_id,
                                      const std::string &name,
                                      reducedsole::uuid &id);

    bool deleteRightGroup(const reducedsole::uuid &id,
                          const reducedsole::uuid &appuser_id,
                          std::string &message);

    bool fetchRightGroup(const reducedsole::uuid &right_group_id,
                         rapidjson::Value &object,
                         rapidjson::MemoryPoolAllocator<> &alloc,
                         std::string &message);

    bool fetchRightGroupMember(const reducedsole::uuid &right_group_id,
                               rapidjson::Value &member,
                               rapidjson::MemoryPoolAllocator<> &alloc,
                               std::string &errorMessage);

    void fetchGroupRightNumbers(const reducedsole::uuid &right_group_id,
                                std::set<int> &right_numbers);

    void fetchAppUserRightNumbers(const reducedsole::uuid &appuser_id,
                                  std::set<int> &right_numbers);

    bool insertRight(const reducedsole::uuid &id,
                     const reducedsole::uuid &right_group_id,
                     const int right_number,
                     std::string &message);

    bool removeRight(const reducedsole::uuid &right_group_id,
                     const int right_number,
                     std::string &message);

    bool removeUser(const reducedsole::uuid &right_group_id,
                    const reducedsole::uuid &appuser_id,
                    std::string &message);

    void checkAndGenerateAdminGroup(const reducedsole::uuid &app_id,
                                    const std::string &adminGroupName,
                                    const std::set<int> &right_numbers);
    bool adminExists(const reducedsole::uuid &app_id,
                     const std::string &adminGroupName);
};

#endif // DATABASELOGICRIGHTGROUP_H
