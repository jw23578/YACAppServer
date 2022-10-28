#include "handleruploadapp.h"

HandlerUploadApp::HandlerUploadApp(DatabaseLogic &databaseLogic,
                                   PistacheServerInterface &serverInterface,
                                   LoggedInUsersContainer &loggedInUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/uploadApp",
                             TypePost,
                             loggedInUsersContainer),
    databaseLogic(databaseLogic)
{

}

void HandlerUploadApp::method()
{
    MACRO_GetMandatoryPostString(appId);
    MACRO_GetMandatoryPostString(json_yacapp);
    MACRO_GetMandatoryPostString(yacpck_base64);

    std::string message;
    if (!databaseLogic.saveApp(userId,
                               appId,
                               json_yacapp,
                               yacpck_base64,
                               message))
    {
        answer(Pistache::Http::Code::Bad_Request, message);
        return;
    }

    answer(Pistache::Http::Code::Ok, "new yacApp stored");
}
