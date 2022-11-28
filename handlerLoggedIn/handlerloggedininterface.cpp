#include "handlerloggedininterface.h"
#include "serverHeader/loginemailheader.h"
#include "serverHeader/logintokenheader.h"
#include "serverHeader/appidheader.h"

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
    if (!getHeaderString<LoginEMailHeader>(loginEMail, true))
    {
        return false;
    }
    if (!getHeaderString<LoginTokenHeader>(loginToken, true))
    {
        return false;
    }
    std::string temp;
    getHeaderString<AppIdHeader>(temp, false);
    appId = sole::rebuild(temp);
    if (!loggedInContainer.isLoggedIn(appId,
                                      loginEMail,
                                      loginToken,
                                      userId))
    {
        answer(Pistache::Http::Code::Bad_Request, "not logged in");
        return false;
    }
    return true;
}
