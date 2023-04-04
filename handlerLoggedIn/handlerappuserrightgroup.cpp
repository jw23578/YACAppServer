#include "handlerappuserrightgroup.h"

HandlerAppUserRightGroup::HandlerAppUserRightGroup(DatabaseLogics &databaseLogics,
                                                   PistacheServerInterface &serverInterface,
                                                   LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
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

void HandlerAppUserRightGroup::method()
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
        MACRO_GetMandatoryUuid(id);
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value rightgroup;
        std::string message;
        if (!dlrg.fetchRightGroup(id,
                                  rightgroup,
                                  document.GetAllocator(),
                                  message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("rightgroup", rightgroup, document.GetAllocator());
        answerOk(true, document);
        return;
    }

    if (isMethod(methodNames.deleteRightGroup))
    {
        if (answerMissingRight(rl.appUserMissesRight(loggedInUserId, Rights::RN_changeRightsGroups)))
        {
            return;
        }
        MACRO_GetMandatoryUuid(id);
        std::string message;
        answerOk(message,
                 dlrg.deleteRightGroup(id,
                                       loggedInUserId,
                                       message));
        return;
    }
}
