#ifndef TESTDATABASELOGICWORKTIME_H
#define TESTDATABASELOGICWORKTIME_H

#include "databaselogic/databaselogics.h"

class TestDatabaseLogicWorktime
{
    LogStatController &logStatController;
    DatabaseLogics &databaseLogics;
public:
    TestDatabaseLogicWorktime(LogStatController &logStatController,
                              DatabaseLogics &databaseLogics);
};

#endif // TESTDATABASELOGICWORKTIME_H
