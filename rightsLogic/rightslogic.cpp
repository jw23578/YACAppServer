#include "rightslogic.h"
#include "extrapidjson.h"
#include "orm_implementions/t0022_right_group2appuser.h"
#include "orm-mapper/orm2postgres.h"

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

int RightsLogic::appUserMissesRight(const sole::uuid &appuser_id,
                                    const RightNumber &rn)
{
    // if right is missing then the right number is returned
    fetchRightsForUser(appuser_id);
    auto appUserRights(appUsers2RightNumbers[appuser_id]);
    if (appUserRights.find(rn.number) == appUserRights.end())
    {
        return rn.number;
    }
    return 0;
}

void RightsLogic::clear()
{
    checkedAppIds.clear();
    appUsers2RightNumbers.clear();
}

void RightsLogic::addUserRights(const sole::uuid &app_id,
                                const sole::uuid &appuser_id,
                                rapidjson::Value &target,
                                rapidjson::MemoryPoolAllocator<> &alloc)
{
    if (checkedAppIds.find(app_id) == checkedAppIds.end())
    {
        dlrg.checkAndGenerateAdminGroup(app_id, Rights::Administrator, Rights::allRightNumbers);
    }

    if (appIdsWhereAdminExists.find(app_id) == appIdsWhereAdminExists.end())
    {
        if (dlrg.adminExists(app_id, Rights::Administrator))
        {
            appIdsWhereAdminExists.insert(app_id);
        }
        else
        {
            sole::uuid right_group_id;
            if (dlrg.fetchIDOfOneRightGroupByName(app_id, Rights::Administrator, right_group_id))
            {
                t0022_right_group2appuser t0022;
                t0022.right_group_id = right_group_id;
                t0022.appuser_id = appuser_id;
                t0022.requested_datetime = TimePointPostgreSqlNow;
                t0022.approved_datetime = TimePointPostgreSqlNow;
                t0022.approved_appuser_id = appuser_id;
                t0022.denied_datetime = TimePointPostgreSqlNull;
                t0022.denied_appuser_id = NullUuid;
                ORM2Postgres orm2postgres(dlrg.pool);
                orm2postgres.insertOrUpdate(t0022);
                appIdsWhereAdminExists.insert(app_id);
            }
        }
    }
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
