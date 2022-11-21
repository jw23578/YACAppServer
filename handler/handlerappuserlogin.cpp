#include "handlerappuserlogin.h"
#include "extmap.h"

HandlerAppUserLogin::HandlerAppUserLogin(DatabaseLogicAppUser &databaseLogicAppUser,
                                         PistacheServerInterface &serverInterface):
          PistacheHandlerInterface(serverInterface,
                                   "/loginAppUser",
                                   TypePost,
                                   TypeNoLoginNeeded),
          databaseLogicAppUser(databaseLogicAppUser)
{

}

void HandlerAppUserLogin::method()
{
    MACRO_GetMandatoryPostString(loginEMail);
    MACRO_GetMandatoryPostString(password);

    if (!ExtString::emailIsValid(loginEMail))
    {
        answer(Pistache::Http::Code::Bad_Request, "this is not a valid email-adress: " + loginEMail);
        return;
    }

    std::string message;
    std::string loginToken;
    if (!databaseLogicAppUser.loginAppUser(loginEMail,
                                           password,
                                           message,
                                           loginToken))
    {
        answer(Pistache::Http::Code::Bad_Request, message);
    }
    else
    {
        MACRO_CreateDataMAP(loginToken);
        answer(Pistache::Http::Code::Ok, "Login successful", data);
    }

}
