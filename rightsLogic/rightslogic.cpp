#include "rightslogic.h"

RightsLogic::RightsLogic(DatabaseLogicRightGroup &dlrg):
    dlrg(dlrg)
{

}

int RightsLogic::appUserMissesRight(const sole::uuid &appuser_id, const RightNumber &rn)
{
    // if right is missing then the right number is returned
    auto it(appUsers2RightNumbers.find(appuser_id));
    if (it == appUsers2RightNumbers.end())
    {
        dlrg.fetchAppUserRightNumbers(appuser_id, appUsers2RightNumbers[appuser_id]);
    }
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
