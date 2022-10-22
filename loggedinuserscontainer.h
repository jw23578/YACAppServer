#ifndef LOGGEDINUSERSCONTAINER_H
#define LOGGEDINUSERSCONTAINER_H

#include "databaselogic.h"

class LoggedInUsersContainer
{
    DatabaseLogic &databaseLogic;
    typedef std::map<std::string, std::chrono::system_clock::time_point> LoggedInUsersMap;
    LoggedInUsersMap loggedInUsers;
public:
    LoggedInUsersContainer(DatabaseLogic &databaseLogic);

    bool isLoggedIn(const std::string &loginEMail,
                    const std::string &loginToken);
};

#endif // LOGGEDINUSERSCONTAINER_H
