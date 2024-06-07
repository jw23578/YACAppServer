#include "pgsqlimplementation.h"
#include "pgcommandtransactor.h"

PGSqlImplementation::PGSqlImplementation(PGConnectionPool &pool):ORMSqlInterface(),
    pool(pool)
{

}

bool PGSqlImplementation::execute(const SqlString &sql)
{
    PGCommandTransactor ct(pool, sql, result);
}

bool PGSqlImplementation::open(const SqlString &sql)
{
    PGCommandTransactor ct(pool, sql, result);
    currentRow = 0;
    return result.size() > 0;
}

size_t PGSqlImplementation::size()
{
    return result.size();
}

bool PGSqlImplementation::next()
{
    if (currentRow + 1 >= size())
    {
        return false;
    }
    currentRow += 1;
    return true;
}

size_t PGSqlImplementation::storeBlob(const std::vector<char> &data)
{
    // FIXME
    return 0;
}

bool PGSqlImplementation::fetchBlob(size_t blobId, std::vector<char> &data)
{
    // FIXME
    return false;
}

size_t PGSqlImplementation::columns()
{
    return result.columns();
}

std::string PGSqlImplementation::columnName(size_t index)
{
    return result.column_name(index);
}

std::optional<std::string> PGSqlImplementation::value(size_t index)
{
    if (isNull(index))
    {
        return  std::nullopt;
    }
    const pqxx::row &row(result[currentRow]);
    return row[index].get<std::string>().value();
}

std::optional<int> PGSqlImplementation::intValue(size_t index)
{
    if (isNull(index))
    {
        return  std::nullopt;
    }
    const pqxx::row &row(result[currentRow]);
    return row[index].get<int>().value();
}

std::optional<std::chrono::system_clock::time_point> PGSqlImplementation::timepointValue(size_t index)
{
    if (isNull(index))
    {
        return  std::nullopt;
    }
    const pqxx::row &row(result[currentRow]);
    return ExtString::toTimepoint(row[index].c_str());
}

bool PGSqlImplementation::isNull(size_t index)
{
    const pqxx::row &row(result[currentRow]);
    return row[index].is_null();
}
