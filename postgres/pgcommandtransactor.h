#ifndef PGCOMMANDTRANSACTOR_H
#define PGCOMMANDTRANSACTOR_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"
#include "pgconnection.h"
#include "orm_implementions/sqlstring.h"

class PGCommandTransactor
{
    PGConnectionPool &pool;
    PGConnection conn;
    SqlString const &sql;
    pqxx::result &result;
    const bool noTransaction;
    bool failed;
    void execAndCommit(pqxx::transaction_base &w,
                       SqlString const &sql);
public:
    PGCommandTransactor(PGConnectionPool &pool,
                        SqlString const &sql,
                        pqxx::result &result);
    PGCommandTransactor(PGConnectionPool &pool,
                        SqlString const &sql,
                        pqxx::result &result,
                        bool noTransaction);
    void operator()();
    bool ok();
};


#endif // PGCOMMANDTRANSACTOR_H
