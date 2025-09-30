#ifndef HANDLERORMOBJECTS_H
#define HANDLERORMOBJECTS_H

#include "handlerloggedininterface.h"
#include "rightsLogic/rightslogic.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "orm_implementions/yacormfactory.h"
#include "pgconnectionpool.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm-mapper/orm2postgres.h"
#include "tablefields.h"
#include "databaselogic/databaselogics.h"

#include "orm_implementions/t0021_right_group.h"
#include "orm_implementions/t0022_right_group2appuser.h"

class HandlerORMObjects : public HandlerLoggedInInterface
{
    t0021_right_group t0021;
    t0022_right_group2appuser t0022;

    void storeObject(YACBaseObject &object);

    const TableFields tableFields;
    DatabaseLogics &databaseLogics;
    YACORMFactory &factory;
    ORM2rapidjson orm2json;
    ORM2Postgres orm2postgres;
    RightsLogic &rightsLogic;
public:
    HandlerORMObjects(DatabaseLogics &databaseLogics,
                      YACORMFactory &factory,
                      PGConnectionPool &pool,
                      RightsLogic &rightsLogic,
                      PistacheServerInterface &serverInterface,
                      LoggedInAppUsersContainer &loggedInAppUsersContainer);

    void method(CurrentContext &context) override;
};

#endif // HANDLERORMOBJECTS_H
