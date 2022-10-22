#ifndef LOGGEDINUSERSCONTAINER_H
#define LOGGEDINUSERSCONTAINER_H

#include "databaselogic.h"

class LoggedInUsersContainer
{
    DatabaseLogic &databaseLogic;
    struct SData
    {
        sole::uuid userId;
        std::chrono::system_clock::time_point loginTokenValidUntil;
    };

    typedef std::map<std::string, SData> LoggedInUsersMap;
    LoggedInUsersMap loggedInUsers;
public:
    LoggedInUsersContainer(DatabaseLogic &databaseLogic);

    bool isLoggedIn(const std::string &loginEMail,
                    const std::string &loginToken,
                    sole::uuid &userId);
};

#endif // LOGGEDINUSERSCONTAINER_H
