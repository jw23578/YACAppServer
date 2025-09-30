#ifndef DATABASELOGICS_H
#define DATABASELOGICS_H

#include "pgconnectionpool.h"
#include "logstatcontroller.h"

#include "databaselogicappuser.h"
#include "databaselogicworktime.h"
#include "databaselogicimagetable.h"
#include "databaselogicmessages.h"
#include "databaselogicrightgroup.h"
#include "databaselogic/databaselogicspaces.h"
#include "databaselogic/databaselogicuserandapp.h"
#include "databaselogicthirdparty.h"
#include "rightsLogic/rightslogic.h"
#include "ormpersistenceinterface.h"

class DatabaseLogics
{
    LogStatController &logStatController;
    ORMPersistenceInterface &opi;

public:
    PGConnectionPool &pool;
    ORMPersistenceInterface &getOpi()
    {
        return opi;
    }

    DatabaseLogicAppUser databaseLogicAppUser;
    DatabaseLogicWorktime databaseLogicWorktime;
    DatabaseLogicImageTable databaseLogicImageTable;
    DatabaseLogicMessages databaseLogicMessages;
    DatabaseLogicRightGroup databaseLogicRightGroup;
    DatabaseLogicSpaces databaseLogicSpaces;
    DatabaseLogicUserAndApp databaseLogicUserAndApp;
    DatabaseLogicThirdParty databaseLogicThirdParty;

    RightsLogic rightsLogic;

    LogStatController &getLogStat();


    DatabaseLogics(LogStatController &logStatController,
                   PGConnectionPool &pool,
                   ORMPersistenceInterface &opi);
};

#endif // DATABASELOGICS_H
