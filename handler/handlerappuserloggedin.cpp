#include "handlerappuserloggedin.h"

HandlerAppUserLoggedIn::HandlerAppUserLoggedIn(LoggedInAppUsersContainer &loggedInAppUsersContainer,
                                               PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/appUserLoggedIn",
                             TypeGet,
                             TypeNoLoginNeeded),
    loggedInAppUsersContainer(loggedInAppUsersContainer)
{

}

void HandlerAppUserLoggedIn::method()
{
    MACRO_GetMandatoryGetString(loginEMail);
    MACRO_GetMandatoryGetString(loginToken);

    if (!ExtString::emailIsValid(loginEMail))
    {
        answer(Pistache::Http::Code::Bad_Request, "this is not a valid email-adress: " + loginEMail);
        return;
    }

    if (loggedInAppUsersContainer.isLoggedIn(loginEMail,
                                             loginToken))
    {
        answer(Pistache::Http::Code::Ok, "user logged in");
        return;
    }
    answer(Pistache::Http::Code::Ok, "user not logged in");
}

