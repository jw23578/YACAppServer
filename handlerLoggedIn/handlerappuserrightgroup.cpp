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
}

void HandlerAppUserRightGroup::method()
{
    DatabaseLogicRightGroup &dlrg(databaseLogics.databaseLogicRightGroup);
    RightsLogic &rl(databaseLogics.rightsLogic);
    if (isMethod(methodNames.fetchRightGroups))
    {
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value rightgroups;
        std::string message;
        if (!dlrg.fetchRightGroups(rightgroups,
                                   document.GetAllocator(),
                                   message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("rightgroups", rightgroups, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    if (isMethod(methodNames.insertRightGroup))
    {
        if (answerMissingRight(rl.appUserMissesRight(loggedInUserId, rl.rights.RN_insertRightGroup)))
        {
            return;
        }
        MACRO_GetMandatoryString(name);
        std::string message;
        answerOk(message,
                 dlrg.insertRightGroup(sole::uuid4(),
                                       name,
                                       loggedInUserId,
                                       message));
        return;
    }

    if (isMethod(methodNames.updateRightGroup))
    {
        if (answerMissingRight(rl.appUserMissesRight(loggedInUserId, rl.rights.RN_updateRightGroup)))
        {
            return;
        }
        MACRO_GetMandatoryUuid(id);
        MACRO_GetMandatoryString(name);
        std::string message;
        answerOk(message,
                 dlrg.updateRightGroup(id,
                                       name,
                                       loggedInUserId,
                                       message));
        return;
    }

    if (isMethod(methodNames.deleteRightGroup))
    {
        if (answerMissingRight(rl.appUserMissesRight(loggedInUserId, rl.rights.RN_deleteRightGroup)))
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
