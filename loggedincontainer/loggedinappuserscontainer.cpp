#include "loggedinappuserscontainer.h"

LoggedInAppUsersContainer::LoggedInAppUsersContainer(DatabaseLogicAppUser &databaseLogicAppUser):
    LoggedInContainerInterface(),
    databaseLogicAppUser(databaseLogicAppUser)
{

}

bool LoggedInAppUsersContainer::isLoggedIn(const sole::uuid &appId,
                                           const std::string &loginEMail,
                                           const std::string &loginToken,
                                           sole::uuid &userId)
{
    std::string needle(loginEMail + "##" + loginToken);
    LoggedInUsersMap::iterator it(loggedInUsers.find(needle));
    if (it == loggedInUsers.end() || it->second.loginTokenValidUntil < std::chrono::system_clock::now())
    {
        std::chrono::system_clock::time_point loginTokenValidUntil;
        if (!databaseLogicAppUser.appUserLoggedIn(appId,
                                                  loginEMail,
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
        databaseLogicAppUser.refreshAppUserLoginToken(appId,
                                                      loginEMail,
                                                      it->second.loginTokenValidUntil);
    }
    userId = it->second.userId;
    return true;
}

bool LoggedInAppUsersContainer::isLoggedIn(const sole::uuid &appId,
                                           const std::string &loginEMail,
                                           const std::string &loginToken)
{
    sole::uuid userId;
    return isLoggedIn(appId, loginEMail, loginToken, userId);
}

bool LoggedInAppUsersContainer::appIdMandatory() const
{
    return true;
}
