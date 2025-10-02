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
private:
    LoggedInUsersMap loggedInUsers;
    std::string generateNeedle(const std::string &loginEMail,
                               const std::string &loginToken,
                               const std::string &third,
                               const std::string &mandant);

public:
    LoggedInContainerInterface();

    virtual bool appIdMandatory() const = 0;

    LoggedInUsersMap::iterator find(const std::string &loginEMail,
                                    const std::string &loginToken,
                                    const std::string &third,
                                    const std::string &mandant);
    LoggedInUsersMap::iterator end();
    LoggedInUsersMap::iterator erase(LoggedInUsersMap::iterator it);

    LoggedInUsersMap::iterator add(const std::string &loginEMail,
                                   const std::string &loginToken,
                                   const std::string &third,
                                   const std::string &mandant,
                                   const reducedsole::uuid &userId,
                                   const std::chrono::system_clock::time_point loginTokenValidUntil);


    virtual bool isLoggedIn(CurrentContext &context,
                            const std::string &loginEMail,
                            const std::string &loginToken,
                            const std::string &third,
                            const std::string &mandant,
                            reducedsole::uuid &userId) = 0;
    virtual bool logout(CurrentContext &context,
                        const std::string &loginEMail,
                        const std::string &loginToken,
                        const std::string &third,
                        const std::string &mandant) = 0;

    bool isLoggedInWithOutUserId(CurrentContext &context,
                                 const std::string &loginEMail,
                                 const std::string &loginToken,
                                 const std::string &third,
                                 const std::string &mandant);

    void clear(const reducedsole::uuid &userId);
    void clearByNeedleData(const std::string &loginEMail,
                           const std::string &loginToken,
                           const std::string &third,
                           const std::string &mandant);

};

#endif // LOGGEDINCONTAINERINTERFACE_H
