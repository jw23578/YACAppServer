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

    bool isLoggedIn(const sole::uuid &appId,
                    const std::string &loginEMail,
                    const std::string &loginToken,
                    sole::uuid &userId) override;

    bool isLoggedIn(const sole::uuid &appId,
                    const std::string &loginEMail,
                    const std::string &loginToken) override;
};

#endif // LOGGEDINAPPUSERSCONTAINER_H
