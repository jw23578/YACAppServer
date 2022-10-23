#include "handleruserloggedin.h"

HandlerUserLoggedIn::HandlerUserLoggedIn(PistacheServerInterface &serverInterface,
                                         LoggedInUsersContainer &loggedInUsersContainer):
    PistacheHandlerInterface(serverInterface,
                             "/userLoggedIn",
                             TypeGet,
                             TypeNoLoginNeeded),
    loggedInUsersContainer(loggedInUsersContainer)
{

}

void HandlerUserLoggedIn::method()
{
    MACRO_GetMandatoryGetString(loginEMail);
    MACRO_GetMandatoryGetString(loginToken);
    if (loggedInUsersContainer.isLoggedIn(loginEMail,
                                          loginToken))
    {
        answer(Pistache::Http::Code::Ok, "user logged in");
        return;
    }
    answer(Pistache::Http::Code::Ok, "user not logged in");
}
