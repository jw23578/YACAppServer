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
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(loginToken);
    MACRO_GetMandatoryUuid(appId);

    if (loggedInAppUsersContainer.isLoggedIn(appId,
                                             loginEMail,
                                             loginToken))
    {
        answerOk("user logged in", true);
        return;
    }
    answerOk("user not logged in", false);
}

