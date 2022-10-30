#include "pgexecutor.h"
#include "pgcommandtransactor.h"
#include "utils/extstring.h"


PGExecutor::PGExecutor(PGConnectionPool &pool,
                       const PGSqlString &sql): pool(pool)
{
    pool.getLC().log(__FILE__,
                     __LINE__,
                     LogStatController::verbose,
                     std::string("Executing: ") + sql.str());
    PGCommandTransactor ct(pool, sql, result);
}

size_t PGExecutor::exec(const PGSqlString &sql)
{
    PGCommandTransactor ct(pool, sql, result);
    return size();
}

size_t PGExecutor::size()
{
    return result.size();
}

size_t PGExecutor::columns()
{
    return result.columns();
}

size_t PGExecutor::next()
{
    return ++currentRow;
}

std::string PGExecutor::columnName(size_t c)
{
    return result.column_name(c);
}

bool PGExecutor::isNull(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return row[fieldname].is_null();
}

bool PGExecutor::isNullOrEmpty(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return row[fieldname].is_null() || row[fieldname].get<std::string>().value() == "";
}

bool PGExecutor::boolean(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return row[fieldname].get<bool>().value();
}

std::string PGExecutor::string(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return row[fieldname].get<std::string>().value();
}

int PGExecutor::integer(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return row[fieldname].get<int>().value();
}

std::chrono::system_clock::time_point PGExecutor::timepoint(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return ExtString::toTimepoint(row[fieldname].c_str());
}

sole::uuid PGExecutor::uuid(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return sole::rebuild(row[fieldname].c_str());
}

pqxx::oid PGExecutor::oid(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return row[fieldname].get<pqxx::oid>().value();
}
