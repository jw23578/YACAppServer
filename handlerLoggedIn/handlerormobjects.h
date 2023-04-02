#ifndef HANDLERORMOBJECTS_H
#define HANDLERORMOBJECTS_H

#include "handlerloggedininterface.h"
#include "rightsLogic/rightslogic.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "orm_implementions/yacormfactory.h"
#include "pgconnectionpool.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm-mapper/orm2postgres.h"

class HandlerORMObjects : public HandlerLoggedInInterface
{
    YACORMFactory &factory;
    ORM2rapidjson orm2json;
    ORM2Postgres orm2postgres;
    RightsLogic &rightsLogic;
public:
    HandlerORMObjects(YACORMFactory &factory,
                      PGConnectionPool &pool,
                      RightsLogic &rightsLogic,
                      PistacheServerInterface &serverInterface,
                      LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method() override;
};

#endif // HANDLERORMOBJECTS_H
