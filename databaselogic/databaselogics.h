#ifndef DATABASELOGICS_H
#define DATABASELOGICS_H

#include "postgres/pgconnectionpool.h"
#include "logstat/logstatcontroller.h"

#include "databaselogicappuser.h"
#include "databaselogicworktime.h"
#include "databaselogicimagetable.h"
#include "databaselogicmessages.h"

class DatabaseLogics
{
    LogStatController &logStatController;
    PGConnectionPool &pool;

public:
    DatabaseLogicAppUser databaseLogicAppUser;
    DatabaseLogicWorktime databaseLogicWorktime;
    DatabaseLogicImageTable databaseLogicImageTable;
    DatabaseLogicMessages databaseLogicMessages;

    DatabaseLogics(LogStatController &logStatController,
                   PGConnectionPool &pool);
};

#endif // DATABASELOGICS_H
