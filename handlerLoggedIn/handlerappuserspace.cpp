#include "handlerappuserspace.h"

HandlerAppUserSpace::HandlerAppUserSpace(DatabaseLogics &databaseLogics,
                                         PistacheServerInterface &serverInterface,
                                         LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
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

void HandlerAppUserSpace::method(CurrentContext &context)
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
        reducedsole::uuid id(ExtUuid::NullUuid);
        dls.fetchSpaceRequestId(space_id, context.userId, id);
        dls.insertOrUpdateSpace2AppUser(id, context.appId, space_id, context.userId, TimePointPostgreSqlNow, TimePointPostgreSqlNull, ExtUuid::NullUuid, TimePointPostgreSqlNull, ExtUuid::NullUuid);
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
        if (!dls.fetchSpaces(context.appId,
                             context.userId,
                             spaces,
                             document.GetAllocator(),
                             message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("spaces", spaces, document.GetAllocator());
        context.userId = context.userId;
        databaseLogics.rightsLogic.addUserRights(context, document, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    if (isMethod(methodNames.insertSpace))
    {
        if (answerMissingRight(rl.appUserMissesRight(context.userId, Rights::RN_changeSpaces)))
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
        reducedsole::uuid id(ExtUuid::NullUuid);
        if (!dls.insertOrUpdateSpace(id,
                                     context.appId,
                                     name,
                                     context.userId,
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
        if (answerMissingRight(rl.appUserMissesRight(context.userId, Rights::RN_changeSpaces)))
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
                                    context.appId,
                                    name,
                                    context.userId,
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
        if (answerMissingRight(rl.appUserMissesRight(context.userId, Rights::RN_changeSpaces)))
        {
            return;
        }
        MACRO_GetMandatoryUuid(id);
        std::string message;
        answerOk(message,
                 dls.deleteSpace(id,
                                 context.userId,
                                 message));
        return;
    }

}
