#ifndef RIGHTSLOGIC_H
#define RIGHTSLOGIC_H

#include "databaselogic/databaselogicrightgroup.h"
#include "yacAppAndServer/rightnumbers.h"

class RightsLogic
{
    DatabaseLogicRightGroup &dlrg;
    std::set<sole::uuid> checkedAppIds;
    std::set<sole::uuid> appIdsWhereAdminExists;
    std::map<sole::uuid, std::set<int>> appUsers2RightNumbers;
    void fetchRightsForUser(const sole::uuid &appuser_id);
public:
    RightsLogic(DatabaseLogicRightGroup &dlrg);

    int appUserMissesRight(const sole::uuid &appuser_id, RightNumber const &rn);
    void clear();

    void addUserRights(const sole::uuid &app_id,
                       const sole::uuid &appuser_id, rapidjson::Value &target,
                       rapidjson::MemoryPoolAllocator<> &alloc);
};

#endif // RIGHTSLOGIC_H
