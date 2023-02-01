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
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetString(password);
    MACRO_GetMandatoryUuid(appId);


    if (databaseLogicAppUser.appUserExists(appId,
                                           loginEMail))
    {
        answerOk("LoginEMail already exists.", false);
        return;
    }
    std::string verifyToken;
    std::string message;
    if (!databaseLogicAppUser.createAppUser(appId,
                                            loginEMail,
                                            password,
                                            message,
                                            verifyToken))
    {
        answerBad(message);
        return;
    }
    emailLogic.sendPleaseVerifyMail(loginEMail, verifyToken);
    answerOk("appuser registered, please verify", true);
}
