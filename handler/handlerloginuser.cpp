#include "handlerloginuser.h"
#include "extmap.h"

HandlerLoginUser::HandlerLoginUser(DatabaseLogic &databaseLogic,
                                   PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/loginUser",
                             TypePost),
    databaseLogic(databaseLogic)
{

}

void HandlerLoginUser::method()
{
    MACRO_GetMandatoryPostString(loginEMail);
    MACRO_GetMandatoryPostString(password);
    std::string message;
    std::string loginToken;
    if (!databaseLogic.loginUser(loginEMail,
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
