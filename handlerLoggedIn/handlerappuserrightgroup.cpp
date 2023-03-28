#include "handlerappuserrightgroup.h"

HandlerAppUserRightGroup::HandlerAppUserRightGroup(DatabaseLogics &databaseLogics,
                                                   PistacheServerInterface &serverInterface,
                                                   LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             methodNames.fetchRightGroups,
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{
    addMethod(serverInterface,
              methodNames.insertRightGroup,
              TypePost);
    addMethod(serverInterface,
              methodNames.updateRightGroup,
              TypePost);
    addMethod(serverInterface,
              methodNames.deleteRightGroup,
              TypePost);
    addMethod(serverInterface,
              methodNames.fetchRightGroup,
              TypeGet);
    addMethod(serverInterface,
              methodNames.fetchRightGroupMember,
              TypeGet);
    addMethod(serverInterface,
              methodNames.insertOrUpdateRightGroup2AppUser,
              TypePost);
}

void HandlerAppUserRightGroup::method()
{
    DatabaseLogicRightGroup &dlrg(databaseLogics.databaseLogicRightGroup);
    RightsLogic &rl(databaseLogics.rightsLogic);
    if (isMethod(methodNames.insertOrUpdateRightGroup2AppUser))
    {
        MACRO_GetUuid(id);
        MACRO_GetMandatoryUuid(right_group_id);
        MACRO_GetMandatoryUuid(appuser_id);
        MACRO_GetTimePointFromISO(requested_datetime);
        MACRO_GetTimePointFromISO(approved_datetime);
        MACRO_GetMandatoryUuid(approved_appuser_id);
        MACRO_GetTimePointFromISO(denied_datetime);
        MACRO_GetUuid(denied_appuser_id);

        std::string errorMessage;
        answerOk(errorMessage, dlrg.insertOrUpdateRightGroup2AppUser(id,
                                                  right_group_id,
                                                  appuser_id,
                                                  requested_datetime,
                                                  approved_datetime,
                                                  approved_appuser_id,
                                                  denied_datetime,
                                                  denied_appuser_id,
                                                  errorMessage));
        return;
    }
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
    if (isMethod(methodNames.fetchRightGroups))
    {
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value rightgroups;
        std::string message;
        if (!dlrg.fetchRightGroups(appId,
                                   rightgroups,
                                   document.GetAllocator(),
                                   message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("rightgroups", rightgroups, document.GetAllocator());
        databaseLogics.rightsLogic.addUserRights(appId, loggedInUserId, document, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    if (isMethod(methodNames.insertRightGroup))
    {
        if (answerMissingRight(rl.appUserMissesRight(loggedInUserId, Rights::RN_changeRightsGroups)))
        {
            return;
        }
        MACRO_GetMandatoryString(name);
        MACRO_GetMandatoryBool(automatic);
        MACRO_GetMandatoryString(access_code);
        MACRO_GetMandatoryBool(request_allowed);
        MACRO_GetMandatoryBool(visible_for_non_members);
        std::string message("rightgroup inserted");
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value rightgroup;
        sole::uuid id(NullUuid);
        if (!dlrg.insertOrUpdateRightGroup(id,
                                           appId,
                                           name,
                                           loggedInUserId,
                                           automatic,
                                           access_code,
                                           request_allowed,
                                           visible_for_non_members,
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

    if (isMethod(methodNames.updateRightGroup))
    {
        if (answerMissingRight(rl.appUserMissesRight(loggedInUserId, Rights::RN_changeRightsGroups)))
        {
            return;
        }
        MACRO_GetMandatoryUuid(id);
        MACRO_GetMandatoryString(name);
        MACRO_GetMandatoryBool(automatic);
        MACRO_GetMandatoryString(access_code);
        MACRO_GetMandatoryBool(request_allowed);
        MACRO_GetMandatoryBool(visible_for_non_members);
        std::string message("rightgroup updated");
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value rightgroup;
        if(!dlrg.insertOrUpdateRightGroup(id,
                                          appId,
                                          name,
                                          loggedInUserId,
                                          automatic,
                                          access_code,
                                          request_allowed,
                                          visible_for_non_members,
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
