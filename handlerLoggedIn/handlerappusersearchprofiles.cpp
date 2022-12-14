#include "handlerappusersearchprofiles.h"

HandlerAppUserSearchProfiles::HandlerAppUserSearchProfiles(PistacheServerInterface &serverInterface,
                                                           DatabaseLogicAppUser &databaseLogicAppUser,
                                                           LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/appUserSearchProfiles",
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogicAppUser(databaseLogicAppUser)
{

}

void HandlerAppUserSearchProfiles::method()
{
    MACRO_GetMandatoryString(needle);
    MACRO_GetInt(limit);
    MACRO_GetInt(offset);

    std::string message;
    rapidjson::Document answer;
    answer.SetObject();
    rapidjson::Value profiles;
    bool success(databaseLogicAppUser.searchProfiles(appId,
                                                     needle,
                                                     limit,
                                                     offset,
                                                     message,
                                                     profiles,
                                                     answer.GetAllocator()));
    answer.AddMember("message", rapidjson::StringRef(message.c_str()), answer.GetAllocator());
    answer.AddMember("profiles", profiles, answer.GetAllocator());
    answerOk(success, answer);
}
