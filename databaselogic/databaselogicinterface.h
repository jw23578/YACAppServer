#ifndef DATABASELOGICINTERFACE_H
#define DATABASELOGICINTERFACE_H

#include "postgres/pgconnectionpool.h"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "orm-mapper/orm2postgres.h"

class DatabaseLogicInterface
{
protected:
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;
    TableFields tableFields;
    ORM2Postgres orm2postgres;
public:
    DatabaseLogicInterface(LogStatController &logStatController,
                           PGConnectionPool &pool);
};

#endif // DATABASELOGICINTERFACE_H
