#ifndef DATABASELOGICS_H
#define DATABASELOGICS_H

#include "postgres/pgconnectionpool.h"
#include "logstat/logstatcontroller.h"

#include "databaselogicappuser.h"
#include "databaselogicworktime.h"

class DatabaseLogics
{
    LogStatController &logStatController;
    PGConnectionPool &pool;

public:
    DatabaseLogicAppUser databaseLogicAppUser;
    DatabaseLogicWorktime databaseLogicWorktime;

    DatabaseLogics(LogStatController &logStatController,
                   PGConnectionPool &pool);
};

#endif // DATABASELOGICS_H
