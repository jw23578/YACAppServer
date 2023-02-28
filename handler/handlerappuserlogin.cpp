#include "handlerappuserlogin.h"
#include "extmap.h"

HandlerAppUserLogin::HandlerAppUserLogin(DatabaseLogics &databaseLogics,
                                         DeviceTokenCache &deviceTokenCache,
                                         PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/loginAppUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogics(databaseLogics),
    deviceTokenCache(deviceTokenCache)
{

}

void HandlerAppUserLogin::method()
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(password);
    MACRO_GetMandatoryUuid(appId);

    std::string message;
    sole::uuid appUserId;
    rapidjson::Document data;
    data.SetObject();
    ExtRapidJSONWriter w(data, data.GetAllocator());
    if (!databaseLogics.databaseLogicAppUser.loginAppUser(appId,
                                                          loginEMail,
                                                          password,
                                                          message,
                                                          w,
                                                          appUserId))
    {
        answerBad(message);
    }
    else
    {
        MACRO_GetString(deviceToken);
        if (deviceToken.size())
        {
            deviceTokenCache.add(appUserId,
                                 deviceToken);
        }
        w.addMember("message", "Login successful");
        databaseLogics.rightsLogic.addUserRights(appUserId, data, data.GetAllocator());
        answerOk(true, data);
    }

}
