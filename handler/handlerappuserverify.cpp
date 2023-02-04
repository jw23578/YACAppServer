#include "handlerappuserverify.h"
#include "extmap.h"

HandlerAppUserVerify::HandlerAppUserVerify(DatabaseLogicAppUser &databaseLogicAppUser,
                                           PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/verifyAppUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogicAppUser(databaseLogicAppUser)
{

}

void HandlerAppUserVerify::method()
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(verifyToken);
    MACRO_GetMandatoryUuid(appId);

    std::string message;
    std::map<std::string, std::string> data;
    if (!databaseLogicAppUser.verifyAppUser(appId,
                                           loginEMail,
                                           verifyToken,
                                           message,
                                           data))
    {
        answerBad(message);
        return;
    }
    answerOk(message, true, data);
}
