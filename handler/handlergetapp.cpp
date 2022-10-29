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
    MACRO_GetMandatoryGetString(app_id);
    rapidjson::Document target;
    databaseLogic.fetchOneApp(app_id, target);
    answer(Pistache::Http::Code::Ok, target);
}
