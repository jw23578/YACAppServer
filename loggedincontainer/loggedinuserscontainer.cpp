#include "loggedinuserscontainer.h"

LoggedInUsersContainer::LoggedInUsersContainer(DatabaseLogicUserAndApp &databaseLogic):
    LoggedInContainerInterface(),
    databaseLogic(databaseLogic)
{

}

bool LoggedInUsersContainer::isLoggedIn(const reducedsole::uuid &appId,
                                        const std::string &loginEMail,
                                        const std::string &loginToken,
                                        const std::string &third,
                                        const std::string &mandant,
                                        reducedsole::uuid &userId)
{
    UNUSED(appId);
    UNUSED(third); // wird hier nicht gebraucht, weil es hier um die user geht die apps anlegen/hochladen können
    UNUSED(mandant);
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

bool LoggedInUsersContainer::appIdMandatory() const
{
    return false;
}
