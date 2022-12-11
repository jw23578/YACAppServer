#ifndef LOGGEDINCONTAINERINTERFACE_H
#define LOGGEDINCONTAINERINTERFACE_H

#include "sole/sole.hpp"
#include <string>
#include <map>
#include <chrono>

class LoggedInContainerInterface
{
protected:
    struct SData
    {
        sole::uuid userId;
        std::chrono::system_clock::time_point loginTokenValidUntil;
    };

    typedef std::map<std::string, SData> LoggedInUsersMap;
    LoggedInUsersMap loggedInUsers;

public:
    LoggedInContainerInterface();

    virtual bool appIdMandatory() const = 0;

    virtual bool isLoggedIn(const sole::uuid &appId,
                            const std::string &loginEMail,
                            const std::string &loginToken,
                            sole::uuid &userId) = 0;

    virtual bool isLoggedIn(const sole::uuid &appId,
                            const std::string &loginEMail,
                            const std::string &loginToken) = 0;

    void clear(const sole::uuid &userId);

};

#endif // LOGGEDINCONTAINERINTERFACE_H
