#include "handlerappuserregister.h"

HandlerAppUserRegister::HandlerAppUserRegister(DatabaseLogicAppUser &databaseLogicAppUser,
                                               EMailLogic &emailLogic,
                                               PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/registerAppUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogicAppUser(databaseLogicAppUser),
    emailLogic(emailLogic)
{

}

void HandlerAppUserRegister::method()
{
    MACRO_GetMandatoryPostString(loginEMail);
    MACRO_GetMandatoryPostString(password);

    if (databaseLogicAppUser.appUserExists(loginEMail))
    {
        answer(Pistache::Http::Code::Bad_Request, "loginEMail already exists and cannot be registered again");
        return;
    }
    std::string verifyToken(databaseLogicAppUser.createAppUser(loginEMail, password));
    emailLogic.sendPleaseVerifyMail(loginEMail, verifyToken);
    answer(Pistache::Http::Code::Ok, "appuser registered, please verify");

}
