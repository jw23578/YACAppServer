#ifndef LOGGEDINAPPUSERSCONTAINER_H
#define LOGGEDINAPPUSERSCONTAINER_H

#include "loggedincontainerinterface.h"
#include "databaselogicappuser.h"

class LoggedInAppUsersContainer: public LoggedInContainerInterface
{
    DatabaseLogicAppUser &databaseLogicAppUser;
public:
    LoggedInAppUsersContainer(DatabaseLogicAppUser &databaseLogicAppUser);

    bool isLoggedIn(const sole::uuid &appId,
                    const std::string &loginEMail,
                    const std::string &loginToken,
                    sole::uuid &userId) override;

    bool isLoggedIn(const sole::uuid &appId,
                    const std::string &loginEMail,
                    const std::string &loginToken) override;

    bool appIdMandatory() const override;
};

#endif // LOGGEDINAPPUSERSCONTAINER_H
