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
    if (isMethod(methodNames.insertRightGroup))
    {
        if (missingRight(rl.appUserMissesRight(loggedInUserId, rl.rights.RN_insertRightGroup)))
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
        if (missingRight(rl.appUserMissesRight(loggedInUserId, rl.rights.RN_updateRightGroup)))
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
        if (missingRight(rl.appUserMissesRight(loggedInUserId, rl.rights.RN_deleteRightGroup)))
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
