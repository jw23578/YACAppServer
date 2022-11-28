#ifndef LOGGEDINUSERSCONTAINER_H
#define LOGGEDINUSERSCONTAINER_H

#include "databaselogicuserandapp.h"
#include "loggedincontainerinterface.h"

class LoggedInUsersContainer: public LoggedInContainerInterface
{
    DatabaseLogicUserAndApp &databaseLogic;
    struct SData
    {
        sole::uuid userId;
        std::chrono::system_clock::time_point loginTokenValidUntil;
    };

    typedef std::map<std::string, SData> LoggedInUsersMap;
    LoggedInUsersMap loggedInUsers;
public:
    LoggedInUsersContainer(DatabaseLogicUserAndApp &databaseLogic);

    bool isLoggedIn(const sole::uuid &appId,
                    const std::string &loginEMail,
                    const std::string &loginToken,
                    sole::uuid &userId) override;

    bool isLoggedIn(const sole::uuid &appId,
                    const std::string &loginEMail,
                    const std::string &loginToken) override;
};

#endif // LOGGEDINUSERSCONTAINER_H
