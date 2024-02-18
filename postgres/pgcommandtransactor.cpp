#include "pgcommandtransactor.h"
#include <iostream>
#include <pqxx/except.hxx>
#include "logstat/logstatcontroller.h"
#include "logstat/beginendtrack.h"


void PGCommandTransactor::execAndCommit(pqxx::transaction_base &w,
                                        PGSqlString const &sql)
{
    try
    {
        if (noTransaction)
        {
            LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("no transaction"));
        }
        LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("sql: ") + sql.str());
        BeginEndTrack bet(__FILE__, __LINE__, "PGCommandTransactor");
        result = w.exec(sql.str());
        LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("resultsize: ") + ExtString::toString(result.size()));
        bet.track(__FILE__, __LINE__);
        w.commit();
    }
    catch (const pqxx::failure &e)
    {
        LogStatController::slog(__FILE__, __LINE__, LogStatController::error, std::string("sql error: ") + e.what());
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
