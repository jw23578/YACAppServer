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
    MACRO_GetMandatoryPostString(loginEMail);
    MACRO_GetMandatoryPostString(verifyToken);

    std::string message;
    std::string loginToken;
    if (databaseLogicUserAndApp.verfiyUser(loginEMail,
                                 verifyToken,
                                 message,
                                 loginToken))
    {
        MACRO_CreateDataMAP(loginToken);
        answer(Pistache::Http::Code::Ok, message, data);
    }
    else
    {
        answer(Pistache::Http::Code::Bad_Request, message);
    }
}
