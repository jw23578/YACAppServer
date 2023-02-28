#include "rightslogic.h"
#include "extrapidjson.h"

void RightsLogic::fetchRightsForUser(const sole::uuid &appuser_id)
{
    auto it(appUsers2RightNumbers.find(appuser_id));
    if (it == appUsers2RightNumbers.end())
    {
        dlrg.fetchAppUserRightNumbers(appuser_id, appUsers2RightNumbers[appuser_id]);
    }
}

RightsLogic::RightsLogic(DatabaseLogicRightGroup &dlrg):
    dlrg(dlrg)
{

}

int RightsLogic::appUserMissesRight(const sole::uuid &appuser_id, const RightNumber &rn)
{
    // if right is missing then the right number is returned
    auto appUserRights(appUsers2RightNumbers[appuser_id]);
    if (appUserRights.find(rn.number) == appUserRights.end())
    {
        return rn.number;
    }
    return 0;
}

void RightsLogic::clear()
{
    appUsers2RightNumbers.clear();
}

void RightsLogic::addUserRights(const sole::uuid &appuser_id,
                                rapidjson::Value &target,
                                rapidjson::MemoryPoolAllocator<> &alloc)
{
    rapidjson::Value rightsArray;
    rightsArray.SetArray();
    fetchRightsForUser(appuser_id);
    auto appUserRights(appUsers2RightNumbers[appuser_id]);
    for (auto const &rn: appUserRights)
    {
        rightsArray.PushBack(rn, alloc);
    }
    target.AddMember("rights", rightsArray, alloc);
}