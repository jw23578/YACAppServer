#include "handlerappuserloggedin.h"
#include "serverHeader/thirdheader.h"
#include "serverHeader/mandantheader.h"

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

    std::string third;
    std::string mandant;
    getHeaderString<ThirdHeader>(third, false);
    getHeaderString<MandantHeader>(mandant, false);


    if (loggedInAppUsersContainer.isLoggedInWithOutUserId(appId,
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

