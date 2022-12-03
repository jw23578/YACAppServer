#include "handlergetapp.h"

HandlerGetAPP::HandlerGetAPP(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                             PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/getAPP",
                             TypeGet,
                             TypeNoLoginNeeded),
    databaseLogicUserAndApp(databaseLogicUserAndApp)
{

}

void HandlerGetAPP::method()
{
    MACRO_GetMandatoryString(app_id);
    MACRO_GetMandatoryInt(current_installed_version, true);
    rapidjson::Document target;
    databaseLogicUserAndApp.fetchOneApp(app_id, current_installed_version, target);
    answerOk(true, target);
}
