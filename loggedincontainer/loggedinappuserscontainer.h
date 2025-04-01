#ifndef LOGGEDINAPPUSERSCONTAINER_H
#define LOGGEDINAPPUSERSCONTAINER_H

#include "loggedincontainerinterface.h"
#include "databaselogics.h"

class LoggedInAppUsersContainer: public LoggedInContainerInterface
{
    DatabaseLogics &databaseLogics;
public:
    LoggedInAppUsersContainer(DatabaseLogics &databaseLogics);

    bool isLoggedIn(const reducedsole::uuid &appId,
                    const std::string &loginEMail,
                    const std::string &loginToken,
                    const std::string &third,
                    const std::string &mandant,
                    reducedsole::uuid &userId) override;

    bool appIdMandatory() const override;
};

#endif // LOGGEDINAPPUSERSCONTAINER_H
