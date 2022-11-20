#include "handleruploadapp.h"

HandlerUploadApp::HandlerUploadApp(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                                   PistacheServerInterface &serverInterface,
                                   LoggedInUsersContainer &loggedInUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/uploadApp",
                             TypePost,
                             loggedInUsersContainer),
    databaseLogicUserAndApp(databaseLogicUserAndApp)
{

}

void HandlerUploadApp::method()
{
    MACRO_GetMandatoryString(app_id);
    MACRO_GetMandatoryString(json_yacapp);
    MACRO_GetMandatoryString(yacpck_base64);
    MACRO_GetMandatoryString(app_name);
    MACRO_GetMandatoryInt(app_version, false);
    MACRO_GetMandatoryString(app_logo_url);
    MACRO_GetMandatoryString(app_color_name);
    MACRO_GetMandatoryBool(is_template_app);

    std::string message;
    if (!databaseLogicUserAndApp.saveApp(userId,
                               app_id,
                               app_name,
                               app_version,
                               app_logo_url,
                               app_color_name,
                               is_template_app,
                               json_yacapp,
                               yacpck_base64,
                               message))
    {
        answer(Pistache::Http::Code::Bad_Request, message);
        return;
    }

    answer(Pistache::Http::Code::Ok, "new yacApp stored");
}
