#ifndef DATABASELOGICS_H
#define DATABASELOGICS_H

#include "postgres/pgconnectionpool.h"
#include "logstat/logstatcontroller.h"

#include "databaselogicappuser.h"
#include "databaselogicworktime.h"
#include "databaselogicimagetable.h"
#include "databaselogicmessages.h"
#include "databaselogicappointments.h"
#include "databaselogicrightgroup.h"
#include "databaselogic/databaselogicspaces.h"
#include "databaselogic/databaselogicuserandapp.h"
#include "databaselogicthirdparty.h"
#include "rightsLogic/rightslogic.h"
#include "orm/ormpersistenceinterface.h"

class DatabaseLogics
{
    LogStatController &logStatController;
    ORMPersistenceInterface &opi;

public:
    PGConnectionPool &pool;

    DatabaseLogicAppUser databaseLogicAppUser;
    DatabaseLogicWorktime databaseLogicWorktime;
    DatabaseLogicImageTable databaseLogicImageTable;
    DatabaseLogicMessages databaseLogicMessages;
    DatabaseLogicAppointments databaseLogicAppointments;
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
