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
    MACRO_GetMandatoryPostString(app_id);
    MACRO_GetMandatoryPostString(json_yacapp);
    MACRO_GetMandatoryPostString(yacpck_base64);
    MACRO_GetMandatoryPostString(app_name);
    MACRO_GetMandatoryPostString(app_logo_url);
    MACRO_GetMandatoryPostString(app_color_name);

    std::string message;
    if (!databaseLogic.saveApp(userId,
                               app_id,
                               app_name,
                               app_logo_url,
                               app_color_name,
                               json_yacapp,
                               yacpck_base64,
                               message))
    {
        answer(Pistache::Http::Code::Bad_Request, message);
        return;
    }

    answer(Pistache::Http::Code::Ok, "new yacApp stored");
}
