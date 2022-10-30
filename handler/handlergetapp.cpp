#include "handlergetapp.h"

HandlerGetAPP::HandlerGetAPP(DatabaseLogic &databaseLogic,
                             PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/getAPP",
                             TypeGet,
                             TypeNoLoginNeeded),
    databaseLogic(databaseLogic)
{

}

void HandlerGetAPP::method()
{
    MACRO_GetMandatoryString(app_id);
    MACRO_GetMandatoryInt(current_installed_version, true);
    rapidjson::Document target;
    databaseLogic.fetchOneApp(app_id, current_installed_version, target);
    answer(Pistache::Http::Code::Ok, target);
}
