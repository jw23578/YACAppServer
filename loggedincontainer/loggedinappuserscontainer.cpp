#include "loggedinappuserscontainer.h"
#include "thirdparties/thirdcurlrequests.h"

LoggedInAppUsersContainer::LoggedInAppUsersContainer(DatabaseLogics &databaseLogics):
    LoggedInContainerInterface(),
    databaseLogics(databaseLogics)
{

}

bool LoggedInAppUsersContainer::isLoggedIn(const reducedsole::uuid &appId,
                                           const std::string &loginEMail,
                                           const std::string &loginToken,
                                           const std::string &third,
                                           const std::string &mandant,
                                           reducedsole::uuid &userId)
{
    std::string needle(loginEMail + "##" + third + "##" + mandant + "##" + loginToken);
    LoggedInUsersMap::iterator it(loggedInUsers.find(needle));
    if (it == loggedInUsers.end() || it->second.loginTokenValidUntil < std::chrono::system_clock::now())
    {
        std::chrono::system_clock::time_point loginTokenValidUntil;
        bool loggedIn(false);
        if (third.size())
        {
            std::string message;
            ThirdCurlRequests tcr;
            loggedIn = tcr.isLoggedIn(databaseLogics.getLogStat(),
                                      third,
                                      mandant,
                                      loginEMail,
                                      loginToken,
                                      message,
                                      loginTokenValidUntil);
            if (loggedIn)
            {
                userId = databaseLogics.databaseLogicAppUser.getAppUserId(appId, loginEMail);
            }
        }
        else
        {
            loggedIn = databaseLogics.databaseLogicAppUser.appUserLoggedIn(appId,
                                                                           loginEMail,
                                                                           loginToken,
                                                                           userId,
                                                                           loginTokenValidUntil);
        }
        if (!loggedIn)
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
        if (third.size())
        {
            ThirdCurlRequests tcr;
            tcr.refreshLoginToken(databaseLogics.getLogStat(),
                                  third,
                                  mandant,
                                  loginEMail,
                                  loginToken,
                                  it->second.loginTokenValidUntil);
        }
        else
        {
            databaseLogics.databaseLogicAppUser.refreshAppUserLoginToken(appId,
                                                                         loginEMail,
                                                                         it->second.loginTokenValidUntil);
        }
    }
    userId = it->second.userId;
    return true;
}

bool LoggedInAppUsersContainer::appIdMandatory() const
{
    return true;
}
