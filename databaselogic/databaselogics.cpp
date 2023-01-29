#include "databaselogics.h"

LogStatController &DatabaseLogics::getLogStat()
{
    return logStatController;
}

DatabaseLogics::DatabaseLogics(LogStatController &logStatController,
                               PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    databaseLogicAppUser(logStatController,
                         pool),
    databaseLogicWorktime(logStatController,
                          pool),
    databaseLogicImageTable(logStatController,
                            pool),
    databaseLogicMessages(logStatController,
                          pool)
{

}
