#ifndef LOGGEDINCONTAINERINTERFACE_H
#define LOGGEDINCONTAINERINTERFACE_H

#include "JWUtils/reducedsole.h"
#include <string>
#include <map>
#include <chrono>
#include "orm_implementions/currentcontext.h"

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

    virtual bool isLoggedIn(CurrentContext &context,
                            const std::string &loginEMail,
                            const std::string &loginToken,
                            const std::string &third,
                            const std::string &mandant,
                            reducedsole::uuid &userId) = 0;
    virtual bool logout(const std::string &loginToken) = 0;

    bool isLoggedInWithOutUserId(CurrentContext &context,
                                 const std::string &loginEMail,
                                 const std::string &loginToken,
                                 const std::string &third,
                                 const std::string &mandant);

    void clear(const reducedsole::uuid &userId);
    void clearByLoginToken(const std::string &loginToken);

};

#endif // LOGGEDINCONTAINERINTERFACE_H
