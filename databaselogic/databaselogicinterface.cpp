#include "databaselogicinterface.h"

DatabaseLogicInterface::DatabaseLogicInterface(LogStatController &logStatController, PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    utils(pool)
{

}
