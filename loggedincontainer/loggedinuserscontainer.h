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

    bool isLoggedIn(const std::string &loginEMail,
                    const std::string &loginToken,
                    sole::uuid &userId);

    bool isLoggedIn(const std::string &loginEMail,
                    const std::string &loginToken);
};

#endif // LOGGEDINUSERSCONTAINER_H
