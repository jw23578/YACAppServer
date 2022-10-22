#include "loggedinuserscontainer.h"

LoggedInUsersContainer::LoggedInUsersContainer(DatabaseLogic &databaseLogic):
    databaseLogic(databaseLogic)
{

}

bool LoggedInUsersContainer::isLoggedIn(const std::string &loginEMail,
                                        const std::string &loginToken)
{
    std::string needle(loginEMail + "##" + loginToken);
    LoggedInUsersMap::iterator it(loggedInUsers.find(needle));
    if (it == loggedInUsers.end() || it->second < std::chrono::system_clock::now())
    {
        std::chrono::system_clock::time_point loginTokenValidUntil;
        if (!databaseLogic.userLoggedIn(loginEMail,
                                        loginToken,
                                        loginTokenValidUntil))
        {
            if (it != loggedInUsers.end())
            {
                loggedInUsers.erase(it);
            }
            return false;
        }
        loggedInUsers[needle] = loginTokenValidUntil;
        it = loggedInUsers.find(needle);
    }
    if (it->second < std::chrono::system_clock::now() + std::chrono::hours(24 * 3))
    {
        databaseLogic.refreshLoginToken(loginEMail,
                                        it->second);
    }
    return true;
}
