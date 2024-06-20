#include "pgsqlimplementation.h"
#include "pgcommandtransactor.h"

PGSqlImplementation::PGSqlImplementation(PGConnectionPool &pool):ORMSqlInterface(),
    pool(pool)
{

}

bool PGSqlImplementation::internalExecute(const SqlString &sql)
{
    PGCommandTransactor ct(pool, sql, result);
    return result.size() > 0;
}

bool PGSqlImplementation::internalOpen(const SqlString &sql)
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

size_t PGSqlImplementation::storeBlob(const std::basic_string<std::byte> &data)
{
    PGConnection conn(pool);
    pqxx::work w(*conn.getConnection());
    pqxx::oid blobId(pqxx::blob::from_buf(w, data.data()));
    w.commit();
    return blobId;
}

bool PGSqlImplementation::fetchBlob(size_t blobId, std::basic_string<std::byte> &data)
{
    PGConnection conn(pool);
    pqxx::work w(*conn.getConnection());
    std::size_t maxRead(1024 * 1024);
        while (pqxx::blob::append_to_buf(w, blobId, data.size(), data, maxRead)) {}
    w.commit();
    return data.size() > 0;
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
