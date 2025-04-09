#ifndef LOGGEDINCONTAINERINTERFACE_H
#define LOGGEDINCONTAINERINTERFACE_H

#include "JWUtils/reducedsole.h"
#include <string>
#include <map>
#include <chrono>

class LoggedInContainerInterface
{
protected:
    struct SData
    {
        reducedsole::uuid userId;
        std::chrono::system_clock::time_point loginTokenValidUntil;
    };

    typedef std::map<std::string, SData> LoggedInUsersMap;
    LoggedInUsersMap loggedInUsers;

public:
    LoggedInContainerInterface();

    virtual bool appIdMandatory() const = 0;

    virtual bool isLoggedIn(const reducedsole::uuid &appId,
                            const std::string &loginEMail,
                            const std::string &loginToken,
                            const std::string &third,
                            const std::string &mandant,
                            reducedsole::uuid &userId) = 0;

    bool isLoggedInWithOutUserId(const reducedsole::uuid &appId,
                                 const std::string &loginEMail,
                                 const std::string &loginToken,
                                 const std::string &third,
                                 const std::string &mandant);

    void clear(const reducedsole::uuid &userId);

};

#endif // LOGGEDINCONTAINERINTERFACE_H
