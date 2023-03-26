#include "handlerappuserspace.h"

HandlerAppUserSpace::HandlerAppUserSpace(DatabaseLogics &databaseLogics,
                                         PistacheServerInterface &serverInterface,
                                         LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             methodNames.fetchSpaces,
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{
    addMethod(serverInterface,
              methodNames.insertSpace,
              TypePost);
    addMethod(serverInterface,
              methodNames.updateSpace,
              TypePost);
    addMethod(serverInterface,
              methodNames.deleteSpace,
              TypePost);
    addMethod(serverInterface,
              methodNames.fetchSpace,
              TypeGet);
    addMethod(serverInterface,
              methodNames.requestSpaceAccess,
              TypePost);
    addMethod(serverInterface,
              methodNames.spaceRequestResultSeen,
              TypePost);
}

void HandlerAppUserSpace::method()
{
    DatabaseLogicSpaces &dls(databaseLogics.databaseLogicSpaces);
    RightsLogic &rl(databaseLogics.rightsLogic);
    if (isMethod(methodNames.spaceRequestResultSeen))
    {
        MACRO_GetMandatoryUuid(id);
        MACRO_GetMandatoryUuid(appuser_id);
        std::string errorMessage;
        if (!dls.spaceRequestResultSeen(id, appuser_id, errorMessage))
        {
            answerOk(errorMessage, false);
            return;
        }
        answerOk("request successful", true);
        return;
    }
    if (isMethod(methodNames.requestSpaceAccess))
    {
        MACRO_GetMandatoryUuid(space_id);
        sole::uuid id(NullUuid);
        dls.fetchSpaceRequestId(space_id, loggedInUserId, id);
        dls.insertOrUpdateSpace2AppUser(id, appId, space_id, loggedInUserId, TimePointPostgreSqlNow, TimePointPostgreSqlNull, NullUuid, TimePointPostgreSqlNull, NullUuid);
        answerOk("request successful", true);
        return;
    }
    if (isMethod(methodNames.fetchSpace))
    {
        MACRO_GetMandatoryUuid(id);
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value space;
        std::string message;
        if (!dls.fetchSpace(id,
                            space,
                            document.GetAllocator(),
                            message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("space", space, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    if (isMethod(methodNames.fetchSpaces))
    {
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value spaces;
        std::string message;
        if (!dls.fetchSpaces(appId,
                             loggedInUserId,
                             spaces,
                             document.GetAllocator(),
                             message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("spaces", spaces, document.GetAllocator());
        databaseLogics.rightsLogic.addUserRights(appId, loggedInUserId, document, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    if (isMethod(methodNames.insertSpace))
    {
        if (answerMissingRight(rl.appUserMissesRight(loggedInUserId, Rights::RN_changeSpaces)))
        {
            return;
        }
        MACRO_GetMandatoryString(name);
        MACRO_GetMandatoryBool(automatic);
        MACRO_GetString(access_code);
        MACRO_GetBool(request_allowed);
        std::string message("space inserted");
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value space;
        sole::uuid id(NullUuid);
        if (!dls.insertOrUpdateSpace(id,
                                     appId,
                                     name,
                                     loggedInUserId,
                                     automatic,
                                     access_code,
                                     request_allowed,
                                     space,
                                     document.GetAllocator(),
                                     message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("space", space, document.GetAllocator());
        answerOk(true, document);
        return;
    }

    if (isMethod(methodNames.updateSpace))
    {
        if (answerMissingRight(rl.appUserMissesRight(loggedInUserId, Rights::RN_changeSpaces)))
        {
            return;
        }
        MACRO_GetMandatoryUuid(id);
        MACRO_GetMandatoryString(name);
        MACRO_GetMandatoryBool(automatic);
        MACRO_GetString(access_code);
        MACRO_GetBool(request_allowed);
        std::string message("space updated");
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value space;
        if(!dls.insertOrUpdateSpace(id,
                                    appId,
                                    name,
                                    loggedInUserId,
                                    automatic,
                                    access_code,
                                    request_allowed,
                                    space,
                                    document.GetAllocator(),
                                    message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("space", space, document.GetAllocator());
        answerOk(true, document);
        return;
    }

    if (isMethod(methodNames.deleteSpace))
    {
        if (answerMissingRight(rl.appUserMissesRight(loggedInUserId, Rights::RN_changeSpaces)))
        {
            return;
        }
        MACRO_GetMandatoryUuid(id);
        std::string message;
        answerOk(message,
                 dls.deleteSpace(id,
                                 loggedInUserId,
                                 message));
        return;
    }

}
