#include "handlerappuserlogin.h"
#include "extmap.h"

HandlerAppUserLogin::HandlerAppUserLogin(DatabaseLogicAppUser &databaseLogicAppUser,
                                         PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/loginAppUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogicAppUser(databaseLogicAppUser)
{

}

void HandlerAppUserLogin::method()
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(password);
    MACRO_GetMandatoryUuid(appId);

    std::string message;
    std::string loginToken;
    if (!databaseLogicAppUser.loginAppUser(appId,
                                           loginEMail,
                                           password,
                                           message,
                                           loginToken))
    {
        answer(Pistache::Http::Code::Bad_Request, message);
    }
    else
    {
        MACRO_CreateDataMAP(loginToken);
        answer(Pistache::Http::Code::Ok, "Login successful", data);
    }

}
