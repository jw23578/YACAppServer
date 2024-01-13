#include "loggedincontainerinterface.h"

LoggedInContainerInterface::LoggedInContainerInterface()
{

}

bool LoggedInContainerInterface::isLoggedInWithOutUserId(const sole::uuid &appId,
                                                         const std::string &loginEMail,
                                                         const std::string &loginToken,
                                                         const std::string &third,
                                                         const std::string &mandant)
{

    sole::uuid userId;
    return isLoggedIn(appId, loginEMail, loginToken, third, mandant, userId);
}

void LoggedInContainerInterface::clear(const sole::uuid &userId)
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
