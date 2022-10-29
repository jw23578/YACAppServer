#include "handlergetallapps.h"

HandlerGetAllApps::HandlerGetAllApps(DatabaseLogic &databaseLogic,
                                     PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/getAllAPPs",
                             TypeGet,
                             TypeNoLoginNeeded),
    databaseLogic(databaseLogic)
{

}

void HandlerGetAllApps::method()
{
    rapidjson::Document allAPPsObject;
    databaseLogic.fetchAllAPPs(allAPPsObject);
    answer(Pistache::Http::Code::Ok, allAPPsObject);
}
