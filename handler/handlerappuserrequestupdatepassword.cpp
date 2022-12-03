#include "handlerappuserrequestupdatepassword.h"

HandlerAppUserRequestUpdatePassword::HandlerAppUserRequestUpdatePassword(DatabaseLogicAppUser &databaseLogicAppUser,
                                                                         EMailLogic &emailLogic,
                                                                         PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/requestUpdatePasswordAppUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogicAppUser(databaseLogicAppUser),
    emailLogic(emailLogic)
{

}

void HandlerAppUserRequestUpdatePassword::method()
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryUuid(appId);

    std::string message;
    std::string updatePasswordToken;
    if (!databaseLogicAppUser.requestUpdatePassword(appId,
                                                    loginEMail,
                                                    updatePasswordToken,
                                                    message))
    {
        answerBad(message);
        return;
    }
    emailLogic.sendPleaseUpdatePasswordMail(loginEMail, updatePasswordToken);
    message = "e-mail with updatePasswordToken sended";
    answerOk(message, true);

}
