#ifndef TESTDATABASELOGICAPPUSER_H
#define TESTDATABASELOGICAPPUSER_H

#include "databaselogic/databaselogicappuser.h"

class TestDatabaseLogicAppUser
{
    bool error{false};
public:
    TestDatabaseLogicAppUser(DatabaseLogicAppUser &databaseLogicAppUser);

    operator bool() const {return error;}
};

#endif // TESTDATABASELOGICAPPUSER_H
