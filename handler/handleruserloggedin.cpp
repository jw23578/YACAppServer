#include "handleruserloggedin.h"
#include "definitions.h"

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
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(loginToken);
    std::string third;
    std::string mandant;
    if (loggedInUsersContainer.isLoggedInWithOutUserId(NullUuid,
                                                       loginEMail,
                                                       loginToken,
                                                       third,
                                                       mandant))
    {
        answerOk("user logged in", true);
        return;
    }
    answerOk("user not logged in", false);
}
