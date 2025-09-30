#include "handlerappuserfetchprofile.h"

HandlerAppUserFetchProfile::HandlerAppUserFetchProfile(DatabaseLogics &databaseLogics,
                                                       PistacheServerInterface &serverInterface,
                                                       LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
                             methodNames.fetchProfile,
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{
    addMethod(serverInterface,
              methodNames.fetchMyProfile,
              TypeGet);
}

void HandlerAppUserFetchProfile::method(CurrentContext &context)
{
    rapidjson::Document profile;
    profile.SetObject();
    if (isMethod(methodNames.fetchMyProfile))
    {
        std::string message;
        if (!databaseLogics.databaseLogicAppUser.fetchMyProfile(context.appId,
                                                                context.userId,
                                                                message,
                                                                profile,
                                                                profile.GetAllocator()))
        {
            answerOk(message, false);
            return;
        }
    }
    if (isMethod(methodNames.fetchProfile))
    {
        MACRO_GetMandatoryUuid(profileId);
        std::string message;
        if (!databaseLogics.databaseLogicAppUser.fetchProfile(context.appId,
                                                              profileId,
                                                              message,
                                                              profile,
                                                              profile.GetAllocator()))
        {
            answerOk(message, false);
            return;
        }
    }
    answerOk(true, profile);
}
