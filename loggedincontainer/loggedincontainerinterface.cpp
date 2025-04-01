#include "loggedincontainerinterface.h"

LoggedInContainerInterface::LoggedInContainerInterface()
{

}

bool LoggedInContainerInterface::isLoggedInWithOutUserId(const reducedsole::uuid &appId,
                                                         const std::string &loginEMail,
                                                         const std::string &loginToken,
                                                         const std::string &third,
                                                         const std::string &mandant)
{

    reducedsole::uuid userId;
    return isLoggedIn(appId, loginEMail, loginToken, third, mandant, userId);
}

void LoggedInContainerInterface::clear(const reducedsole::uuid &userId)
{
    auto it(loggedInUsers.begin());
    while (it != loggedInUsers.end())
    {
        if (it->second.userId == userId)
        {
            it = loggedInUsers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
