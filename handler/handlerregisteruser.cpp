#include "handlerregisteruser.h"

HandlerRegisterUser::HandlerRegisterUser(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                                 EMailLogic &emailLogic,
                                 PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/registerUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogicUserAndApp(databaseLogicUserAndApp),
    emailLogic(emailLogic)
{

}

void HandlerRegisterUser::method()
{
    MACRO_GetMandatoryPostString(loginEMail);
    MACRO_GetMandatoryPostString(password);

    if (databaseLogicUserAndApp.userExists(loginEMail))
    {
        answer(Pistache::Http::Code::Bad_Request, "loginEMail already exists and cannot be registered again");
        return;
    }
    std::string verifyToken(databaseLogicUserAndApp.createUser(loginEMail, password));
    emailLogic.sendPleaseVerifyMail(loginEMail, verifyToken);
    answer(Pistache::Http::Code::Ok, "user registered, please verify");
}
