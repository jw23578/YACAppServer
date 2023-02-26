#ifndef TESTDATABASELOGICS_H
#define TESTDATABASELOGICS_H

#include "databaselogic/databaselogics.h"

class TestDatabaseLogics
{
    bool error{false};
public:
    TestDatabaseLogics(DatabaseLogics &databaseLogics);
    operator bool() const {return error;}
};

#endif // TESTDATABASELOGICS_H
