#include "handlerregister.h"

HandlerRegister::HandlerRegister(DatabaseLogic &databaseLogic,
                                 PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/register",
                             TypePost),
    databaseLogic(databaseLogic)
{

}

void HandlerRegister::method()
{
    MACRO_GetMandatoryPostString(loginEMail);
    MACRO_GetMandatoryPostString(password);

    if (databaseLogic.userExists(loginEMail))
    {
        answer(Pistache::Http::Code::Bad_Request, "loginEMail already exists and cannot be registered again");
        return;
    }
    std::cout << databaseLogic.createUser(loginEMail, password);
    answer(Pistache::Http::Code::Ok, "user registered, please verify");
}
