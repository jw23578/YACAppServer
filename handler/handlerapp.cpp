#include "handlerapp.h"

HandlerAPP::HandlerAPP(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                       PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/getAPP",
                             TypeGet,
                             TypeNoLoginNeeded),
    databaseLogicUserAndApp(databaseLogicUserAndApp)
{
    addMethod(serverInterface,
              methodNames.getAllAPPs,
              TypeGet);
}

void HandlerAPP::method()
{
    if (isMethod(methodNames.getAllAPPs))
    {
        rapidjson::Document allAPPsObject;
        databaseLogicUserAndApp.getAllAPPs(allAPPsObject);
        answerOk(true, allAPPsObject);
        return;
    }

    MACRO_GetMandatoryString(app_id);
    MACRO_GetMandatoryInt(current_installed_version, true);
    rapidjson::Document target;
    databaseLogicUserAndApp.fetchOneApp(app_id, current_installed_version, target);
    answerOk(true, target);
}
