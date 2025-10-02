#include "loggedinappuserscontainer.h"
#include "thirdparties/thirdcurlrequests.h"
#include "orm_implementions/t0004_user_logintoken.h"

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
    LoggedInUsersMap::iterator it(find(loginEMail, loginToken, third, mandant));
    if (it == end() || it->second.loginTokenValidUntil < std::chrono::system_clock::now())
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
            t0004_user_logintoken userLoginToken;
            loggedIn = userLoginToken.userLoggedIn(context, loginEMail, loginToken);
            userId = userLoginToken.user_id;
            loginTokenValidUntil = userLoginToken.login_token_valid_until;
        }
        if (!loggedIn)
        {
            if (it != end())
            {
                erase(it);
            }
            return false;
        }
        it = add(loginEMail, loginToken, third, mandant, userId, loginTokenValidUntil);
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
            t0004_user_logintoken userLoginToken;
            userLoginToken.refresh(context,
                                   loginToken);
            it->second.loginTokenValidUntil = userLoginToken.login_token_valid_until;
        }
    }
    userId = it->second.userId;
    return true;
}

bool LoggedInAppUsersContainer::logout(CurrentContext &context,
                                       const std::string &loginEMail,
                                       const std::string &loginToken,
                                       const std::string &third,
                                       const std::string &mandant)
{
    clearByNeedleData(loginEMail,
                      loginToken,
                      third,
                      mandant);
    t0004_user_logintoken::logoutUserByLoginToken(context, loginToken);
    return true;
}


bool LoggedInAppUsersContainer::appIdMandatory() const
{
    return true;
}
