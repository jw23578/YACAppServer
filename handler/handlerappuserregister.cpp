#include "handlerappuserregister.h"

HandlerAppUserRegister::HandlerAppUserRegister(DatabaseLogics &databaseLogics,
                                               EMailLogic &emailLogic,
                                               PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             methodNames.registerAppUser,
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogics(databaseLogics),
    emailLogic(emailLogic)
{
    addMethod(serverInterface,
              methodNames.requestVerifyToken,
              TypePost);
}

void HandlerAppUserRegister::method()
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryUuid(appId);
    if (isMethod(methodNames.requestVerifyToken))
    {
        std::string verifyToken;
        std::string message;
        if (!databaseLogics.databaseLogicAppUser.createVerifyToken(appId,
                                                                   loginEMail,
                                                                   message,
                                                                   verifyToken))
        {
            answerOk(message, false);
            return;
        }
        emailLogic.sendVerifyTokenMail(loginEMail, verifyToken);
        answerOk("verifyToken created and sended",
                 true);
        return;
    }
    MACRO_GetString(password);


    std::string verifyToken;
    std::string message;
    if (!databaseLogics.databaseLogicAppUser.createAppUser(appId,
                                                           loginEMail,
                                                           password,
                                                           message,
                                                           verifyToken))
    {
        answerOk(message, false);
        return;
    }
    emailLogic.sendPleaseVerifyMail(loginEMail, verifyToken);
    answerOk("appuser registered, please verify", true);
}
