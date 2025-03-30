#include "handlerappuserverify.h"
#include "extmap.h"

HandlerAppUserVerify::HandlerAppUserVerify(DatabaseLogics &databaseLogics,
                                           PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/verifyAppUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogics(databaseLogics)
{

}

void HandlerAppUserVerify::method()
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(verifyToken);
    MACRO_GetMandatoryUuid(appId);

    std::string message;
    rapidjson::Document data;
    data.SetObject();
    ExtRapidJSONWriter w(data, data.GetAllocator());
    sole::uuid appUserId;
    if (!databaseLogics.databaseLogicAppUser.verifyAppUser(appId,
                                                           loginEMail,
                                                           verifyToken,
                                                           message,
                                                           w,
                                                           appUserId))
    {
        answerOk(message, false);
        return;
    }
    w.addMember("message", message);
    databaseLogics.rightsLogic.addUserRights(appId, appUserId, data, data.GetAllocator());
    answerOk(true, data);
}
