#include "pgexecutor.h"
#include "pgcommandtransactor.h"
#include "utils/extstring.h"


PGExecutor::PGExecutor(PGConnectionPool &pool,
                       const PGSqlString &sql)
{
    PGCommandTransactor ct(pool, sql, result);
}

size_t PGExecutor::size()
{
    return result.size();
}

std::string PGExecutor::string(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return row[fieldname].get<std::string>().value();
}

std::chrono::system_clock::time_point PGExecutor::timepoint(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return ExtString::toTimepoint(row[fieldname].c_str());
}
