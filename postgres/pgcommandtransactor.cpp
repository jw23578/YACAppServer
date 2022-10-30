#include "pgcommandtransactor.h"
#include <iostream>
#include <pqxx/except.hxx>


PGCommandTransactor::PGCommandTransactor(PGConnectionPool &pool,
                                         PGSqlString const &sql,
                                         pqxx::result &result):
    pool(pool),
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
    pool(pool),
    conn(pool),
    sql(sql),
    result(result),
    failed(false)
{
    if (noTransaction)
    {
        pqxx::nontransaction w(*conn.getConnection());
        try
        {
            result = w.exec(sql.str());
            w.commit();
        }
        catch (const pqxx::failure &e)
        {
            pool.getLC().log(__FILE__, __LINE__, LogStatController::error, std::string("sql error: ") + e.what());
            failed = true;
        }
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
        pool.getLC().log(__FILE__, __LINE__, LogStatController::error, std::string("sql error: ") + e.what());
        failed = true;
    }
}

bool PGCommandTransactor::ok()
{
    return !failed;
}
