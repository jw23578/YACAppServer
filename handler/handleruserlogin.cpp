#include "handleruserlogin.h"
#include "extmap.h"

HandlerUserLogin::HandlerUserLogin(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                                   PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/loginUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogicUserAndApp(databaseLogicUserAndApp)
{

}

void HandlerUserLogin::method()
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(password);

    std::string message;
    std::string loginToken;
    if (!databaseLogicUserAndApp.loginUser(loginEMail,
                                           password,
                                           message,
                                           loginToken))
    {
        answerBad(message);
    }
    else
    {
        MACRO_CreateDataMAP(loginToken);
        answerOk("Login successful", true, data);
    }
}
