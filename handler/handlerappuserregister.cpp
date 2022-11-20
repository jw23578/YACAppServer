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
    MACRO_GetMandatoryPostString(appId);

    if (!ExtString::emailIsValid(loginEMail))
    {
        answer(Pistache::Http::Code::Bad_Request, "this is not a valid email-adress: " + loginEMail);
        return;
    }

    if (databaseLogicAppUser.appUserExists(loginEMail))
    {
        answer(Pistache::Http::Code::Bad_Request, "loginEMail already exists and cannot be registered again");
        return;
    }
    std::string verifyToken;
    std::string message;
    if (!databaseLogicAppUser.createAppUser(sole::rebuild(appId),
                                            loginEMail,
                                            password,
                                            message,
                                            verifyToken))
    {
        answer(Pistache::Http::Code::Bad_Request, message);
        return;
    }
    emailLogic.sendPleaseVerifyMail(loginEMail, verifyToken);
    answer(Pistache::Http::Code::Ok, "appuser registered, please verify");
}
