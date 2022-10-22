#include "handlerverifyuser.h"
#include "extmap.h"

HandlerVerifyUser::HandlerVerifyUser(DatabaseLogic &databaseLogic,
                                     PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/verifyUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogic(databaseLogic)
{

}

void HandlerVerifyUser::method()
{
    MACRO_GetMandatoryPostString(loginEMail);
    MACRO_GetMandatoryPostString(verifyToken);

    std::string message;
    std::string loginToken;
    if (databaseLogic.verfiyUser(loginEMail,
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
