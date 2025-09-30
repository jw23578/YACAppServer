#include "loggedinappuserscontainer.h"
#include "thirdparties/thirdcurlrequests.h"

LoggedInAppUsersContainer::LoggedInAppUsersContainer(ORMPersistenceInterface &opi,
                                                     DatabaseLogics &databaseLogics):
    LoggedInContainerInterface(),
    opi(opi),
    databaseLogics(databaseLogics)
{

}

bool LoggedInAppUsersContainer::isLoggedIn(CurrentContext &context,
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
                t0002_user userProfile;
                userProfile.loadByLoginEMail(context, loginEMail);
                userId = userProfile.user_id;
            }
        }
        else
        {
            loggedIn = databaseLogics.databaseLogicAppUser.appUserLoggedIn(context,
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
            databaseLogics.databaseLogicAppUser.refreshAppUserLoginToken(context,
                                                                         loginEMail,
                                                                         it->second.loginTokenValidUntil);
        }
    }
    userId = it->second.userId;
    return true;
}

bool LoggedInAppUsersContainer::logout(const std::string &loginToken)
{
    clearByLoginToken(loginToken);
    databaseLogics.databaseLogicAppUser.logoutAppUserByLoginToken(loginToken);
    return true;
}


bool LoggedInAppUsersContainer::appIdMandatory() const
{
    return true;
}
