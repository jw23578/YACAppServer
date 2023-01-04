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
    MACRO_GetMandatoryUuid(userId);

    rapidjson::Document profile;
    std::string message;
    if (databaseLogics.databaseLogicAppUser.fetchProfile(appId,
                                                         userId,
                                                         message,
                                                         profile,
                                                         profile.GetAllocator()))
    {
        answerOk(message, false);
        return;
    }
    answerOk(true, profile);
}
