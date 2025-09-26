#ifndef LOGGEDINAPPUSERSCONTAINER_H
#define LOGGEDINAPPUSERSCONTAINER_H

#include "loggedincontainerinterface.h"
#include "databaselogics.h"
#include "ormpersistenceinterface.h"

class LoggedInAppUsersContainer: public LoggedInContainerInterface
{
    ORMPersistenceInterface &opi;
    DatabaseLogics &databaseLogics;
public:
    LoggedInAppUsersContainer(ORMPersistenceInterface &opi,
                              DatabaseLogics &databaseLogics);

    bool isLoggedIn(const reducedsole::uuid &appId,
                    const std::string &loginEMail,
                    const std::string &loginToken,
                    const std::string &third,
                    const std::string &mandant,
                    reducedsole::uuid &userId) override;
    bool logout(const std::string &loginToken) override;

    bool appIdMandatory() const override;
};

#endif // LOGGEDINAPPUSERSCONTAINER_H
