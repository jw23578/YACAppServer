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
    std::map<std::string, std::string> data;
    sole::uuid appUserId;
    if (!databaseLogics.databaseLogicAppUser.loginAppUser(appId,
                                                          loginEMail,
                                                          password,
                                                          message,
                                                          data,
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
        answerOk("Login successful", true, data);
    }

}
