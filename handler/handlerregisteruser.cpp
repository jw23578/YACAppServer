#include "handlerregisteruser.h"

HandlerRegisterUser::HandlerRegisterUser(DatabaseLogic &databaseLogic,
                                 EMailLogic &emailLogic,
                                 PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/registerUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogic(databaseLogic),
    emailLogic(emailLogic)
{

}

void HandlerRegisterUser::method()
{
    MACRO_GetMandatoryPostString(loginEMail);
    MACRO_GetMandatoryPostString(password);

    if (databaseLogic.userExists(loginEMail))
    {
        answer(Pistache::Http::Code::Bad_Request, "loginEMail already exists and cannot be registered again");
        return;
    }
    std::string verifyToken(databaseLogic.createUser(loginEMail, password));
    emailLogic.sendPleaseVerifyMail(loginEMail, verifyToken);
    answer(Pistache::Http::Code::Ok, "user registered, please verify");
}
