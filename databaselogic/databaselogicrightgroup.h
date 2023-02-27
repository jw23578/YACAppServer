#ifndef DATABASELOGICRIGHTGROUP_H
#define DATABASELOGICRIGHTGROUP_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"

class DatabaseLogicRightGroup
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    TableNames tableNames;
    TableFields tableFields;

    bool checkRightGroupCreator(const sole::uuid &id,
                                const sole::uuid &creater_id,
                                std::string &message);

public:
    DatabaseLogicRightGroup(LogStatController &logStatController,
                            PGConnectionPool &pool);

    bool insertRightGroup(const sole::uuid &id,
                          const std::string &name,
                          const sole::uuid &creater_id,
                          std::string &message);

    bool updateRightGroup(const sole::uuid &id,
                          const std::string &name,
                          const sole::uuid &creater_id,
                          std::string &message);

    bool deleteRightGroup(const sole::uuid &id,
                          const sole::uuid &creater_id,
                          std::string &message);

    bool fetchRightGroups(rapidjson::Value &targetArray,
                          rapidjson::MemoryPoolAllocator<> &alloc,
                          std::string &message);

    void fetchAppUserRightNumbers(const sole::uuid &appuser_id,
                                  std::set<int> &right_numbers);

    bool insertRight(const sole::uuid &id,
                     const sole::uuid &right_group_id,
                     const int right_number,
                     std::string &message);

    bool removeRight(const sole::uuid &right_group_id,
                     const int right_number,
                     std::string &message);
};

#endif // DATABASELOGICRIGHTGROUP_H
