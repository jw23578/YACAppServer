#include "handlerappuserupdateprofile.h"

HandlerAppUserUpdateProfile::HandlerAppUserUpdateProfile(PistacheServerInterface &serverInterface,
                                                         DatabaseLogicAppUser &databaseLogicAppUser,
                                                         LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/updateAppUserProfile",
                             TypePost,
                             loggedInAppUsersContainer),
    databaseLogicAppUser(databaseLogicAppUser)
{

}

void HandlerAppUserUpdateProfile::method()
{
    MACRO_GetMandatoryString(fstname);
    MACRO_GetMandatoryString(surname);
    MACRO_GetMandatoryString(visible_name);
    MACRO_GetMandatoryBool(searching_exactly_allowed);
    MACRO_GetMandatoryBool(searching_fuzzy_allowed);

    std::string message;
    if (!databaseLogicAppUser.updateAppUser(appId,
                                       userId,
                                       fstname,
                                       surname,
                                       visible_name,
                                       searching_exactly_allowed,
                                       searching_fuzzy_allowed,
                                       message))
    {
        answerOk(message, false);
        return;
    }
    answerOk(message, true);
}
