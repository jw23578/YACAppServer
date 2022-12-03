#include "handlerverifyuser.h"
#include "extmap.h"

HandlerVerifyUser::HandlerVerifyUser(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                                     PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/verifyUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogicUserAndApp(databaseLogicUserAndApp)
{

}

void HandlerVerifyUser::method()
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(verifyToken);

    std::string message;
    std::string loginToken;
    if (databaseLogicUserAndApp.verifyUser(loginEMail,
                                           verifyToken,
                                           message,
                                           loginToken))
    {
        MACRO_CreateDataMAP(loginToken);
        answerOk(message, true, data);
    }
    else
    {
        answerBad(message);
    }
}
