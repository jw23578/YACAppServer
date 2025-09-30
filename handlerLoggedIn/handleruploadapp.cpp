#include "handleruploadapp.h"
#include "orm_implementions/t0001_apps.h"
#include "orm-mapper/orm2rapidjson.h"

HandlerUploadApp::HandlerUploadApp(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                                   ORMPersistenceInterface &opi,
                                   PistacheServerInterface &serverInterface,
                                   LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               opi,
                             methodNames.uploadApp,
                             TypePost,
                             loggedInAppUsersContainer),
    dlua(databaseLogicUserAndApp)
{
    addMethod(serverInterface, t0027.getORMName(), TypePost);
    addMethod(serverInterface, t0027.getORMName(), TypeGet);
    addMethod(serverInterface, t0027.getORMName(), TypeDelete);
}

void HandlerUploadApp::method(CurrentContext &context)
{
    std::string errorMessage;
    ORM2rapidjson orm2json;
    if (isMethod(t0027.getORMName()))
    {
        orm2json.fromJson(getPostedData(), t0027);
        bool appExists(false);
        if (!dlua.userIsAppOwner(t0027.app_id, context.userId, errorMessage, appExists))
        {
            answerOk(errorMessage, false);
            return;
        }
        if (!appExists)
        {
            answerOk("app does not exist", false);
            return;
        }
        if (isPost())
        {
            dlua.storeAppImage(t0027);
            answerOk("image stored", true);
            return;
        }
        if (isGet())
        {
            // FIXME FIX ME AND IMPLEMENT ME
        }
        if (isDelete())
        {
            // FIXME FIX ME AND IMPLEMENT ME
        }
        return;
    }
    MACRO_GetString(installation_code);

    t0001_apps app;
    if (!orm2json.fromJson(getPostedData(), app))
    {
        answerOk("data is not an app-object", false);
        return;
    }

    if (!dlua.saveApp(context.userId, app, installation_code, errorMessage))
    {
        answerOk(errorMessage, false);
        return;
    }

    answerOk("new yacApp stored", true);
}
