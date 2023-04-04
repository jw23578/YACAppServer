#include "handleruploadapp.h"
#include "orm_implementions/t0002_apps.h"
#include "orm-mapper/orm2rapidjson.h"

HandlerUploadApp::HandlerUploadApp(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                                   PistacheServerInterface &serverInterface,
                                   LoggedInUsersContainer &loggedInUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/uploadApp",
                             TypePost,
                             loggedInUsersContainer),
    dlua(databaseLogicUserAndApp)
{
    addMethod(serverInterface, t0027.getORMName(), TypePost);
    addMethod(serverInterface, t0027.getORMName(), TypeGet);
    addMethod(serverInterface, t0027.getORMName(), TypeDelete);
}

void HandlerUploadApp::method()
{
    std::string errorMessage;
    ORM2rapidjson orm2json;
    if (isMethod(t0027.getORMName()))
    {
        orm2json.fromJson(getPostedData(), t0027);
        bool appExists(false);
        if (!dlua.userIsAppOwner(t0027.app_id, loggedInUserId, errorMessage, appExists))
        {
            answerOk(errorMessage, false);
        }
        if (!appExists)
        {
            answerOk("app does not exist", false);
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

    t0002_apps app;
    if (!orm2json.fromJson(getPostedData(), app))
    {
        answerOk("data is not an app-object", false);
        return;
    }

    if (!dlua.saveApp(loggedInUserId, app, errorMessage))
    {
        answerOk(errorMessage, false);
        return;
    }

    answerOk("new yacApp stored", true);
}
