#include "rightslogic.h"
#include "orm_implementions/t0022_right_group2appuser.h"
#include "orm-mapper/orm2postgres.h"
#include "orm_implementions/t0021_right_group.h"

void RightsLogic::fetchRightsForUser(const reducedsole::uuid &appuser_id)
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

int RightsLogic::appUserMissesRight(const reducedsole::uuid &appuser_id,
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
    appUsers2RightNumbers.clear();
}

void RightsLogic::addUserRights(CurrentContext &context,
                                rapidjson::Value &target,
                                rapidjson::MemoryPoolAllocator<> &alloc)
{
    t0021_right_group::checkAndGenerateAdminGroup(context);

    if (appIdsWhereAdminExists.find(context.appId) == appIdsWhereAdminExists.end())
    {
        if (dlrg.adminExists(context.appId, Rights::Administrator))
        {
            appIdsWhereAdminExists.insert(context.appId);
        }
        else
        {
            t0021_right_group administratorRightGroup;
            if (administratorRightGroup.load(context, {{administratorRightGroup.name.name(), Rights::Administrator}}))
            {
                t0022_right_group2appuser t0022;
                t0022.right_group_id = administratorRightGroup.right_group_id;
                t0022.user_id = context.userId;
                t0022.requested_datetime = TimePointPostgreSqlNow;
                t0022.approved_datetime = TimePointPostgreSqlNow;
                t0022.approved_appuser_id = context.userId;
                t0022.denied_datetime.setNull(true);
                t0022.denied_appuser_id = ExtUuid::NullUuid;
                ORM2Postgres orm2postgres(dlrg.pool);
                orm2postgres.insertOrUpdate(t0022);
                appIdsWhereAdminExists.insert(context.appId);
            }
        }
    }
    rapidjson::Value rightsArray;
    rightsArray.SetArray();
    fetchRightsForUser(context.userId);
    auto appUserRights(appUsers2RightNumbers[context.userId]);
    for (auto const &rn: appUserRights)
    {
        rightsArray.PushBack(rn, alloc);
    }
    target.AddMember("rights", rightsArray, alloc);
}
