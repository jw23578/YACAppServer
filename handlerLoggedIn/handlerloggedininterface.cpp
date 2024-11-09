#include "handlerloggedininterface.h"
#include "serverHeader/loginemailheader.h"
#include "serverHeader/logintokenheader.h"
#include "serverHeader/appidheader.h"
#include "serverHeader/thirdheader.h"
#include "serverHeader/mandantheader.h"

HandlerLoggedInInterface::HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                                                   LoggedInContainerInterface &loggedInContainer):
    PistacheHandlerInterface(serverInterface, TypeLoginNeeded),
    loggedInContainer(loggedInContainer)
{

}

HandlerLoggedInInterface::HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                                                   const std::string &methodName,
                                                   HandlerType type,
                                                   LoggedInContainerInterface &loggedInContainer):
    PistacheHandlerInterface(serverInterface, methodName, type, TypeLoginNeeded),
    loggedInContainer(loggedInContainer)
{

}

bool HandlerLoggedInInterface::checkLogin()
{
    if (!getHeaderStringEMail<LoginEMailHeader>(loginEMail, true))
    {
        return false;
    }
    if (!getHeaderString<LoginTokenHeader>(loginToken, true))
    {
        return false;
    }
    std::string temp;
    if (!getHeaderString<AppIdHeader>(temp, loggedInContainer.appIdMandatory()))
    {
        return false;
    }
    appId = sole::rebuild(temp);
    getHeaderString<ThirdHeader>(third, false);
    getHeaderString<MandantHeader>(mandant, false);


    if (!loggedInContainer.isLoggedIn(appId,
                                      loginEMail,
                                      loginToken,
                                      third,
                                      mandant,
                                      loggedInUserId))
    {
        answerBad("not logged in");
        return false;
    }
    return true;
}
