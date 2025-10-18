#include "handleruploadapp.h"
#include "orm_implementions/t0001_apps.h"
#include "orm-mapper/orm2rapidjson.h"
#include "base64.h"

HandlerUploadApp::HandlerUploadApp(ORMPersistenceInterface &opi,
                                   PistacheServerInterface &serverInterface,
                                   LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             opi,
                             methodNames.uploadApp,
                             TypePost,
                             loggedInAppUsersContainer)
{
}

void HandlerUploadApp::method(CurrentContext &context)
{
    if (context.appId != t0001_apps::theCreatorApp.app_id)
    {
        answerOk("Only the CreatorApp can Upload Apps", false);
        return;
    }
    std::string errorMessage;
    ORM2rapidjson orm2json;
    MACRO_GetString(installation_code);

    t0001_apps app;
    if (!orm2json.fromJson(getPostedData(), app))
    {
        answerOk("data is not an app-object", false);
        return;
    }

    if (!app.saveApp(context, app, installation_code, errorMessage))
    {
        answerOk(errorMessage, false);
        return;
    }

    answerOk("new yacApp stored", true);
}
