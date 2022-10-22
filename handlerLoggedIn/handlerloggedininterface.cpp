#include "handlerloggedininterface.h"
#include "serverHeader/loginemailheader.h"
#include "serverHeader/logintokenheader.h"

HandlerLoggedInInterface::HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                                                   const std::string &methodName,
                                                   HandlerType type,
                                                   LoggedInUsersContainer &loggedInUsersContainer):
    PistacheHandlerInterface(serverInterface, methodName, type, TypeNoLoginNeeded),
    loggedInUsersContainer(loggedInUsersContainer)
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
    if (!loggedInUsersContainer.isLoggedIn(loginEMail,
                                           loginToken))
    {
        answer(Pistache::Http::Code::Bad_Request, "not logged in");
        return false;
    }
    return true;
}
