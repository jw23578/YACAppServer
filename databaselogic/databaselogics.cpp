#include "databaselogics.h"

DatabaseLogics::DatabaseLogics(LogStatController &logStatController,
                               PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    databaseLogicAppUser(logStatController,
                         pool),
    databaseLogicWorktime(logStatController,
                          pool),
    databaseLogicImageTable(logStatController,
                            pool)
{

}
