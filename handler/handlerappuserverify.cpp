#include "handlerappuserverify.h"
#include "extmap.h"

HandlerAppUserVerify::HandlerAppUserVerify(DatabaseLogicAppUser &databaseLogicAppUser,
                                           PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/verifyAppUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogicAppUser(databaseLogicAppUser)
{

}

void HandlerAppUserVerify::method()
{
    MACRO_GetMandatoryPostString(loginEMail);
    MACRO_GetMandatoryPostString(verifyToken);

    std::string message;
    std::string loginToken;
    if (databaseLogicAppUser.verifyAppUser(loginEMail,
                                           verifyToken,
                                           message,
                                           loginToken))
    {
        MACRO_CreateDataMAP(loginToken);
        answer(Pistache::Http::Code::Ok, message, data);
    }
    else
    {
        answer(Pistache::Http::Code::Bad_Request, message);
    }
}
