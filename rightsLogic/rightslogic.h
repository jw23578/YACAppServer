#ifndef RIGHTSLOGIC_H
#define RIGHTSLOGIC_H

#include "databaselogic/databaselogicrightgroup.h"
#include "yacAppAndServer/rightnumbers.h"
#include "orm_implementions/currentcontext.h"

class RightsLogic
{
    DatabaseLogicRightGroup &dlrg;
    std::set<reducedsole::uuid> appIdsWhereAdminExists;
    std::map<reducedsole::uuid, std::set<int>> appUsers2RightNumbers;
    void fetchRightsForUser(const reducedsole::uuid &appuser_id);
public:
    RightsLogic(DatabaseLogicRightGroup &dlrg);

    int appUserMissesRight(const reducedsole::uuid &appuser_id, RightNumber const &rn);
    void clear();

    void addUserRights(CurrentContext &context,
                       rapidjson::Value &target,
                       rapidjson::MemoryPoolAllocator<> &alloc);
};

#endif // RIGHTSLOGIC_H
