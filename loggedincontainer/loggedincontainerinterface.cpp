#include "loggedincontainerinterface.h"

LoggedInContainerInterface::LoggedInContainerInterface()
{

}

LoggedInContainerInterface::LoggedInUsersMap::iterator LoggedInContainerInterface::add(const std::string &loginEMail,
                                                                                       const std::string &loginToken,
                                                                                       const std::string &third,
                                                                                       const std::string &mandant,
                                                                                       const reducedsole::uuid &userId,
                                                                                       const std::chrono::system_clock::time_point loginTokenValidUntil)
{
    std::string needle(generateNeedle(loginEMail, loginToken, third, mandant));
    loggedInUsers[needle].userId = userId;
    loggedInUsers[needle].loginTokenValidUntil = loginTokenValidUntil;
    return  loggedInUsers.find(needle);
}

LoggedInContainerInterface::LoggedInUsersMap::iterator LoggedInContainerInterface::end()
{
    return loggedInUsers.end();
}

LoggedInContainerInterface::LoggedInUsersMap::iterator LoggedInContainerInterface::erase(LoggedInContainerInterface::LoggedInUsersMap::iterator it)
{
    return loggedInUsers.erase(it);
}


LoggedInContainerInterface::LoggedInUsersMap::iterator LoggedInContainerInterface::find(const std::string &loginEMail, const std::string &loginToken, const std::string &third, const std::string &mandant)
{
    return loggedInUsers.find(generateNeedle(loginEMail, loginToken, third, mandant));
}

std::string LoggedInContainerInterface::generateNeedle(const std::string &loginEMail, const std::string &loginToken, const std::string &third, const std::string &mandant)
{
    return loginEMail + "##" + third + "##" + mandant + "##" + loginToken;
}

bool LoggedInContainerInterface::isLoggedInWithOutUserId(CurrentContext &context,
                                                         const std::string &loginEMail,
                                                         const std::string &loginToken,
                                                         const std::string &third,
                                                         const std::string &mandant)
{

    reducedsole::uuid userId;
    return isLoggedIn(context, loginEMail, loginToken, third, mandant, userId);
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

void LoggedInContainerInterface::clearByNeedleData(const std::string &loginEMail,
                                                   const std::string &loginToken,
                                                   const std::string &third,
                                                   const std::string &mandant)
{
    auto it(loggedInUsers.find(generateNeedle(loginEMail, loginToken, third, mandant)));
    if (it == loggedInUsers.end())
    {
        return;
    }
    loggedInUsers.erase(it);
}
