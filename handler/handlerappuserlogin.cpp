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
    std::map<std::string, std::string> data;
    if (!databaseLogicAppUser.loginAppUser(appId,
                                           loginEMail,
                                           password,
                                           message,
                                           data))
    {
        answerBad(message);
    }
    else
    {
        answerOk("Login successful", true, data);
    }

}
