#ifndef HANDLERORMOBJECTS_H
#define HANDLERORMOBJECTS_H

#include "handlerloggedininterface.h"
#include "rightsLogic/rightslogic.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "orm_implementions/ormfactory.h"
#include "pgconnectionpool.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm-mapper/orm2postgres.h"

class HandlerORMObjects : public HandlerLoggedInInterface
{
    ORMFactory &factory;
    ORM2rapidjson o2json;
    ORM2Postgres o2postgres;
    RightsLogic &rightsLogic;
public:
    HandlerORMObjects(ORMFactory &factory,
                      PGConnectionPool &pool,
                      RightsLogic &rightsLogic,
                      PistacheServerInterface &serverInterface,
                      LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method() override;
};

#endif // HANDLERORMOBJECTS_H
