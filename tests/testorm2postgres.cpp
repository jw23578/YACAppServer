#include "testorm2postgres.h"
#include "orm2postgres.h"
#include "orm_implementions/t0009_appuser_logintoken.h"

TestORM2Postgres::TestORM2Postgres(PGConnectionPool &pool)
{
    ORM2Postgres o2p(pool);
    t0009_appuser_logintoken ghost;
    std::set<ORMObjectInterface*> allT0009;
    o2p.selectAll(ghost, allT0009);

    int z = 1;
}
