#include "handlerloggedininterface.h"
#include "serverHeader/loginemailheader.h"
#include "serverHeader/logintokenheader.h"
#include "serverHeader/appidheader.h"
#include "serverHeader/thirdheader.h"
#include "serverHeader/mandantheader.h"

HandlerLoggedInInterface::HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                                                   ORMPersistenceInterface &opi,
                                                   LoggedInContainerInterface &loggedInContainer):
    PistacheHandlerInterface(serverInterface, opi, TypeLoginNeeded),
    loggedInContainer(loggedInContainer)
{

}

HandlerLoggedInInterface::HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                                                   ORMPersistenceInterface &opi,
                                                   const std::string &methodName,
                                                   HandlerType type,
                                                   LoggedInContainerInterface &loggedInContainer):
    PistacheHandlerInterface(serverInterface, opi, methodName, type, TypeLoginNeeded),
    loggedInContainer(loggedInContainer)
{

}

bool HandlerLoggedInInterface::checkLogin(CurrentContext &context)
{
    if (!getHeaderStringEMail<LoginEMailHeader>(loginEMail, true))
    {
        return false;
    }
    if (!getHeaderString<LoginTokenHeader>(loginToken, true))
    {
        return false;
    }
    getHeaderString<ThirdHeader>(third, false);
    getHeaderString<MandantHeader>(mandant, false);


    if (!loggedInContainer.isLoggedIn(context,
                                      loginEMail,
                                      loginToken,
                                      third,
                                      mandant,
                                      context.userId))
    {
        answerBad("not logged in");
        return false;
    }
    return true;
}

bool HandlerLoggedInInterface::logout()
{
    return loggedInContainer.logout(loginToken);
}
