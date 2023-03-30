#ifndef TESTORM2POSTGRES_H
#define TESTORM2POSTGRES_H

#include "pgconnectionpool.h"


class TestORM2Postgres
{
    bool error{false};

public:
    TestORM2Postgres(PGConnectionPool &pool);

    operator bool() const {return error;}
};

#endif // TESTORM2POSTGRES_H
