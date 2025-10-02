#include "handlerappuserrightgroup.h"
#include "orm_implementions/t0021_right_group.h"

HandlerAppUserRightGroup::HandlerAppUserRightGroup(DatabaseLogics &databaseLogics,
                                                   PistacheServerInterface &serverInterface,
                                                   LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             databaseLogics.getOpi(),
                             "",
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{
    addMethod(serverInterface,
              methodNames.deleteRightGroup,
              TypePost);
    addMethod(serverInterface,
              methodNames.fetchRightGroup,
              TypeGet);
    addMethod(serverInterface,
              methodNames.fetchRightGroupMember,
              TypeGet);
}

void HandlerAppUserRightGroup::method(CurrentContext &context)
{
    DatabaseLogicRightGroup &dlrg(databaseLogics.databaseLogicRightGroup);
    RightsLogic &rl(databaseLogics.rightsLogic);
    if (isMethod(methodNames.fetchRightGroupMember))
    {
        MACRO_GetMandatoryUuid(right_group_id);
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value member;
        std::string errorMessage;
        if (!dlrg.fetchRightGroupMember(right_group_id,
                                        member,
                                        document.GetAllocator(),
                                        errorMessage))
        {
            answerOk(errorMessage, false);
            return;
        }
        document.AddMember("member", member, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    if (isMethod(methodNames.fetchRightGroup))
    {
        MACRO_GetMandatoryUuid(right_group_id);
        t0021_right_group rightGroup;
        if (!rightGroup.load(context, right_group_id))
        {
            answerOk("rightGroup does not extist", false);
            return;
        }
        std::set<int> rightNumbersSet;
        rightGroup.fetchRightNumbers(context.opi, rightNumbersSet);
        std::set<ORMUuid> memberSet;
        rightGroup.fetchMember(context, memberSet);

        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value rightgroup;
        ORM2rapidjson orm2json;
        orm2json.toJson(rightGroup, rightgroup, document.GetAllocator());

        ExtRapidJSONWriter erw(rightgroup, document.GetAllocator());
        erw.addArray("rightNumbers", rightNumbersSet);
        erw.addArray("member", memberSet);

        document.AddMember("rightgroup", rightgroup, document.GetAllocator());
        answerOk(true, document);
        return;
    }

    if (isMethod(methodNames.deleteRightGroup))
    {
        if (answerMissingRight(rl.appUserMissesRight(context, context.userId, Rights::RN_changeRightsGroups)))
        {
            return;
        }
        MACRO_GetMandatoryUuid(right_group_id);
        t0021_right_group rightGroup;
        if (!rightGroup.load(context, right_group_id))
        {
            answerOk("rightGroup does not extist", false);
            return;
        }
        rightGroup.erase(context);
        answerOk("rightGroup deleted", true);
        return;
    }
}
