#include "loggedinappuserscontainer.h"

LoggedInAppUsersContainer::LoggedInAppUsersContainer(DatabaseLogicAppUser &databaseLogicAppUser):
    databaseLogicAppUser(databaseLogicAppUser)
{

}

bool LoggedInAppUsersContainer::isLoggedIn(const std::string &loginEMail,
                                           const std::string &loginToken,
                                           sole::uuid &userId)
{
    std::string needle(loginEMail + "##" + loginToken);
    LoggedInUsersMap::iterator it(loggedInUsers.find(needle));
    if (it == loggedInUsers.end() || it->second.loginTokenValidUntil < std::chrono::system_clock::now())
    {
        std::chrono::system_clock::time_point loginTokenValidUntil;
        if (!databaseLogicAppUser.appUserLoggedIn(loginEMail,
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
        databaseLogicAppUser.refreshAppUserLoginToken(loginEMail,
                                                      it->second.loginTokenValidUntil);
    }
    userId = it->second.userId;
    return true;
}

bool LoggedInAppUsersContainer::isLoggedIn(const std::string &loginEMail,
                                           const std::string &loginToken)
{
    sole::uuid userId;
    return isLoggedIn(loginEMail, loginToken, userId);
}

