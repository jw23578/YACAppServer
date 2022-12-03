#include "handleruserregister.h"

HandlerUserRegister::HandlerUserRegister(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
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

void HandlerUserRegister::method()
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(password);

    if (databaseLogicUserAndApp.userExists(loginEMail))
    {
        answerBad("loginEMail already exists and cannot be registered again");
        return;
    }
    std::string verifyToken(databaseLogicUserAndApp.createUser(loginEMail, password));
    emailLogic.sendPleaseVerifyMail(loginEMail, verifyToken);
    answerOk("user registered, please verify", true);
}
