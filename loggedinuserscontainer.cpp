#include "loggedinuserscontainer.h"

LoggedInUsersContainer::LoggedInUsersContainer(DatabaseLogic &databaseLogic):
    databaseLogic(databaseLogic)
{

}

bool LoggedInUsersContainer::isLoggedIn(const std::string &loginEMail,
                                        const std::string &loginToken,
                                        sole::uuid &userId)
{
    std::string needle(loginEMail + "##" + loginToken);
    LoggedInUsersMap::iterator it(loggedInUsers.find(needle));
    if (it == loggedInUsers.end() || it->second.loginTokenValidUntil < std::chrono::system_clock::now())
    {
        std::chrono::system_clock::time_point loginTokenValidUntil;
        if (!databaseLogic.userLoggedIn(loginEMail,
                                        loginToken,
                                        userId,
                                        loginTokenValidUntil))
        {
            if (it != loggedInUsers.end())
            {
                loggedInUsers.erase(it);
            }
            return false;
        }
        loggedInUsers[needle].userId = userId;
        loggedInUsers[needle].loginTokenValidUntil = loginTokenValidUntil;
        it = loggedInUsers.find(needle);
    }
    if (it->second.loginTokenValidUntil < std::chrono::system_clock::now() + std::chrono::hours(24 * 3))
    {
        databaseLogic.refreshLoginToken(loginEMail,
                                        it->second.loginTokenValidUntil);
    }
    userId = it->second.userId;
    return true;
}

bool LoggedInUsersContainer::isLoggedIn(const std::string &loginEMail,
                                        const std::string &loginToken)
{
    sole::uuid userId;
    return isLoggedIn(loginEMail, loginToken, userId);
}
