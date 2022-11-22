#ifndef LOGGEDINAPPUSERSCONTAINER_H
#define LOGGEDINAPPUSERSCONTAINER_H

#include "loggedincontainerinterface.h"
#include "databaselogicappuser.h"

class LoggedInAppUsersContainer: public LoggedInContainerInterface
{
    DatabaseLogicAppUser &databaseLogicAppUser;
    struct SData
    {
        sole::uuid userId;
        std::chrono::system_clock::time_point loginTokenValidUntil;
    };

    typedef std::map<std::string, SData> LoggedInUsersMap;
    LoggedInUsersMap loggedInUsers;
public:
    LoggedInAppUsersContainer(DatabaseLogicAppUser &databaseLogicAppUser);

    bool isLoggedIn(const std::string &loginEMail,
                    const std::string &loginToken,
                    sole::uuid &userId);

    bool isLoggedIn(const std::string &loginEMail,
                    const std::string &loginToken);
};

#endif // LOGGEDINAPPUSERSCONTAINER_H
