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
    MACRO_GetMandatoryString(password);
    MACRO_GetMandatoryUuid(appId);


    if (databaseLogicAppUser.appUserExists(appId,
                                           loginEMail))
    {
        answerBad("loginEMail already exists and cannot be registered again");
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
