#include "databaselogics.h"

LogStatController &DatabaseLogics::getLogStat()
{
    return logStatController;
}

DatabaseLogics::DatabaseLogics(LogStatController &logStatController,
                               PGConnectionPool &pool,
                               ORMPersistenceInterface &opi):
    logStatController(logStatController),
    opi(opi),
    pool(pool),
    databaseLogicAppUser(logStatController,
                         pool,
                         opi),
    databaseLogicWorktime(logStatController,
                          pool),
    databaseLogicImageTable(logStatController,
                            pool),
    databaseLogicMessages(logStatController,
                          pool),
    databaseLogicRightGroup(logStatController,
                            pool),
    databaseLogicSpaces(logStatController,
                        pool),
    databaseLogicUserAndApp(logStatController,
                            pool),
    databaseLogicThirdParty(logStatController,
                            pool),
    rightsLogic(databaseLogicRightGroup)
{

}
