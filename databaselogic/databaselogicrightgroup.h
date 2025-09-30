#ifndef DATABASELOGICRIGHTGROUP_H
#define DATABASELOGICRIGHTGROUP_H

#include "pgconnectionpool.h"
#include "JWUtils/reducedsole.h"
#include "logstatcontroller.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"
#include <set>
#include "JWUtils/definitions.h"

class DatabaseLogicRightGroup
{
    LogStatController &logStatController;
    TableNames tableNames;
    TableFields tableFields;

public:
    bool appuserInRightGroup(const reducedsole::uuid &right_group_id,
                             const reducedsole::uuid &appuser_id);
    PGConnectionPool &pool;
    DatabaseLogicRightGroup(LogStatController &logStatController,
                            PGConnectionPool &pool);

    bool fetchRightGroupMember(const reducedsole::uuid &right_group_id,
                               rapidjson::Value &member,
                               rapidjson::MemoryPoolAllocator<> &alloc,
                               std::string &errorMessage);

    void fetchAppUserRightNumbers(const reducedsole::uuid &appuser_id,
                                  std::set<int> &right_numbers);

    bool removeRight(const reducedsole::uuid &right_group_id,
                     const int right_number,
                     std::string &message);

    bool removeUser(const reducedsole::uuid &right_group_id,
                    const reducedsole::uuid &appuser_id,
                    std::string &message);

    bool adminExists(const reducedsole::uuid &app_id,
                     const std::string &adminGroupName);
};

#endif // DATABASELOGICRIGHTGROUP_H
