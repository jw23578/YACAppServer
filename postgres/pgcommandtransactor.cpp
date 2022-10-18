#include "pgcommandtransactor.h"
#include <iostream>
#include <pqxx/except.hxx>


PGCommandTransactor::PGCommandTransactor(PGConnectionPool &pool,
                                         PGSqlString const &sql,
                                         pqxx::result &result):
    conn(pool),
    sql(sql),
    result(result),
    failed(false)
{
    pqxx::perform(*this);
}

PGCommandTransactor::PGCommandTransactor(PGConnectionPool &pool,
                                         const PGSqlString &sql,
                                         pqxx::result &result,
                                         bool noTransaction):
    conn(pool),
    sql(sql),
    result(result),
    failed(false)
{
    if (noTransaction)
    {
        pqxx::nontransaction w(*conn.getConnection());
        result = w.exec(sql.str());
        w.commit();
        return;
    }
    pqxx::perform(*this);
}

void PGCommandTransactor::operator()()
{
    pqxx::work w(*conn.getConnection());
    try
    {
        result = w.exec(sql.str());
        w.commit();
    }
    catch (const pqxx::failure &e)
    {
        failed = true;
    }
}

bool PGCommandTransactor::ok()
{
    return !failed;
}
