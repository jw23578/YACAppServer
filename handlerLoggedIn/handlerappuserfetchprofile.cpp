#include "handlerappuserfetchprofile.h"

HandlerAppUserFetchProfile::HandlerAppUserFetchProfile(DatabaseLogics &databaseLogics,
                                                       PistacheServerInterface &serverInterface,
                                                       LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/fetchProfile",
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{

}

void HandlerAppUserFetchProfile::method()
{
    MACRO_GetMandatoryUuid(profileId);

    rapidjson::Document profile;
    profile.SetObject();
    std::string message;
    if (!databaseLogics.databaseLogicAppUser.fetchProfile(appId,
                                                          profileId,
                                                          message,
                                                          profile,
                                                          profile.GetAllocator()))
    {
        answerOk(message, false);
        return;
    }
    answerOk(true, profile);
}
