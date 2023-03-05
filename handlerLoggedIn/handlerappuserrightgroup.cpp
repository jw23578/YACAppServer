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
}

void HandlerAppUserRightGroup::method()
{
    DatabaseLogicRightGroup &dlrg(databaseLogics.databaseLogicRightGroup);
    RightsLogic &rl(databaseLogics.rightsLogic);
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
        std::string message("rightgroup updated");
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value rightgroup;
        if(!dlrg.insertOrUpdateRightGroup(id,
                                          appId,
                                          name,
                                          loggedInUserId,
                                          automatic,
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
