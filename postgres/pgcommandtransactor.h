#ifndef PGCOMMANDTRANSACTOR_H
#define PGCOMMANDTRANSACTOR_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"
#include "pgconnection.h"
#include "pgsqlstring.h"

class PGCommandTransactor
{
    PGConnection conn;
    PGSqlString const &sql;
    pqxx::result &result;
    bool failed;
public:
    PGCommandTransactor(PGConnectionPool &pool,
                        PGSqlString const &sql,
                        pqxx::result &result);
    PGCommandTransactor(PGConnectionPool &pool,
                        PGSqlString const &sql,
                        pqxx::result &result,
                        bool noTransaction);
    void operator()();
    bool ok();
};


#endif // PGCOMMANDTRANSACTOR_H
