#include "pgcommandtransactor.h"
#include <iostream>
#include <pqxx/except.hxx>


void PGCommandTransactor::execAndCommit(pqxx::transaction_base &w,
                                        PGSqlString const &sql)
{
    try
    {
        if (noTransaction)
        {
            pool.getLC().log(__FILE__, __LINE__, LogStatController::verbose, std::string("no transaction"));
        }
        pool.getLC().log(__FILE__, __LINE__, LogStatController::verbose, std::string("sql: ") + sql.str());
        result = w.exec(sql.str());
        pool.getLC().log(__FILE__, __LINE__, LogStatController::verbose, std::string("resultsize: ") + ExtString::toString(result.size()));
        w.commit();
    }
    catch (const pqxx::failure &e)
    {
        pool.getLC().log(__FILE__, __LINE__, LogStatController::error, std::string("sql error: ") + e.what());
        failed = true;
    }
}

PGCommandTransactor::PGCommandTransactor(PGConnectionPool &pool,
                                         PGSqlString const &sql,
                                         pqxx::result &result):
    pool(pool),
    conn(pool),
    sql(sql),
    result(result),
    noTransaction(false),
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
    noTransaction(noTransaction),
    failed(false)
{
    pqxx::perform(*this);
}

void PGCommandTransactor::operator()()
{
    if (noTransaction)
    {
        pqxx::nontransaction w(*conn.getConnection());
        execAndCommit(w, sql);
        return;
    }

    pqxx::work w(*conn.getConnection());
    execAndCommit(w, sql);
}

bool PGCommandTransactor::ok()
{
    return !failed;
}
