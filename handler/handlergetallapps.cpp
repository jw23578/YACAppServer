#include "handlergetallapps.h"

HandlerGetAllApps::HandlerGetAllApps(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                                     PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/getAllAPPs",
                             TypeGet,
                             TypeNoLoginNeeded),
    databaseLogicUserAndApp(databaseLogicUserAndApp)
{

}

void HandlerGetAllApps::method()
{
    rapidjson::Document allAPPsObject;
    databaseLogicUserAndApp.getAllAPPs(allAPPsObject);
    answerOk(true, allAPPsObject);
}
