#include "pgexecutor.h"
#include "pgcommandtransactor.h"
#include "utils/extstring.h"
#include "definitions.h"


PGExecutor::PGExecutor(PGConnectionPool &pool):
    pool(pool)
{

}

PGExecutor::PGExecutor(PGConnectionPool &pool,
                       const SqlString &sql): pool(pool)
{
    PGCommandTransactor ct(pool, sql, result);
}

PGExecutor::PGExecutor(PGConnectionPool &pool,
                       const SqlString &sql,
                       std::string &resultMessage,
                       const std::string &onSizeMessage,
                       const std::string &onZeroSizeMessage):
    pool(pool)
{
    PGCommandTransactor ct(pool, sql, result);
    if (sql.isUpdateStatement())
    {
        resultMessage = affected_rows() > 0 ? onSizeMessage : onZeroSizeMessage;
        return;
    }
    resultMessage = size() > 0 ? onSizeMessage : onZeroSizeMessage;
}

size_t PGExecutor::exec(const SqlString &sql)
{
    PGCommandTransactor ct(pool, sql, result);
    return size();
}

size_t PGExecutor::login(const std::string &tableName,
                         const std::string &passwordHashfield,
                         const std::string &password,
                         const std::string &loginField,
                         const std::string &loginValue)
{
    SqlString sql("select *, ");
    sql += passwordHashfield;
    sql += " = crypt(:password, ";
    sql += passwordHashfield;
    sql += ") as login_ok from ";
    sql += tableName;
    sql += " where ";
    sql += loginField + " = :" + loginField;
    sql.set(loginField, loginValue);
    sql.set("password", password);
    return exec(sql);
}

size_t PGExecutor::select(const std::string &tableName,
                          const std::string &needleField,
                          const std::string &needleValue,
                          const std::string &needleField2,
                          const std::string &needleValue2)
{
    SqlString sql("select * from ");
    sql += tableName;
    sql += " where " + needleField;
    sql += " = :" + needleField;
    sql.set(needleField, needleValue);
    sql += " and " + needleField2;
    sql += " = :" + needleField2;
    sql.set(needleField2, needleValue2);
    return exec(sql);
}

size_t PGExecutor::select(const std::string &tableName,
                          const std::string &needleField,
                          const std::string &needleValue,
                          const std::string &needleField2,
                          const std::string &needleValue2,
                          const std::string &needleField3,
                          const std::chrono::system_clock::time_point &needleValue3)
{
    SqlString sql("select * from ");
    sql += tableName;
    sql.addCompare("where", needleField, "=", needleValue);
    sql.addCompare("and", needleField2, "=", needleValue2);
    sql.addCompare("and", needleField3, needleValue3 == TimePointPostgreSqlNull ? "is" : "=", needleValue3);
    return exec(sql);
}

bool PGExecutor::defaultSelect(const std::string &tableName,
                               const sole::uuid &id,
                               std::string &message)
{
    SqlString sql;
    sql.select(tableName);
    sql.addCompare("where", "id", "=", id);
    exec(sql);
    if (size() == 0)
    {
        message = "no element found";
        return false;
    }
    if (size() > 1)
    {
        message = "fatal error, more than one element found";
        return false;
    }
    return true;
}

void PGExecutor::delet(const std::string &tableName, const std::string &needleField, const std::string &needleValue)
{
    SqlString sql("delete from ");
    sql += tableName;
    sql += " where " + needleField;
    sql += " = :" + needleField;
    sql.set(needleField, needleValue);
    exec(sql);
}

void PGExecutor::defaultDelete(const std::string &table_name,
                               const sole::uuid &id)
{
    delet(table_name, "id", id.str());
}

size_t PGExecutor::size() const
{
    return result.size();
}

bool PGExecutor::resultAvailable() const
{
    return currentRow < size();
}

size_t PGExecutor::affected_rows() const
{
    return result.affected_rows();
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

bool PGExecutor::isNull(const std::string &fieldname) const
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

std::string PGExecutor::string(const pqxx::row::size_type columnNumber) const
{
    const pqxx::row &row(result[currentRow]);
    if (row[columnNumber].is_null())
    {
        return "";
    }
    return row[columnNumber].get<std::string>().value();
}

std::string PGExecutor::string(const std::string &fieldname) const
{
    const pqxx::row &row(result[currentRow]);
    const pqxx::row::size_type columnNumber(row.column_number(fieldname));
    return string(columnNumber);
}

int PGExecutor::integer(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return row[fieldname].get<int>().value();
}

size_t PGExecutor::get_size_t(const std::string &fieldname)
{
    const pqxx::row &row(result[currentRow]);
    return row[fieldname].get<size_t>().value();
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

size_t PGExecutor::array(const std::string &fieldname, std::set<std::string> &target)
{
    const pqxx::row &row(result[currentRow]);
    auto array(row[fieldname].as_array());
    auto elem(array.get_next());
    while (elem.first != pqxx::array_parser::juncture::done )
    {
        if (elem.first == pqxx::array_parser::juncture::string_value)
        {
            target.insert(elem.second.c_str());
        }
        elem = array.get_next();
    }
    return target.size();
}

size_t PGExecutor::fill(std::set<int> &target, const std::string &fieldname)
{
    target.clear();
    return append(target, fieldname);
}

size_t PGExecutor::append(std::set<int> &target, const std::string &fieldname)
{
    for (size_t i(0); i < size(); ++i)
    {
        target.insert(integer(fieldname));
        next();
    }
    return target.size();
}

size_t PGExecutor::fill(rapidjson::Value &targetArray, rapidjson::MemoryPoolAllocator<> &alloc, const std::string &fieldname)
{
    targetArray.Clear();
    append(targetArray, alloc, fieldname);
    return targetArray.Size();
}

size_t PGExecutor::append(rapidjson::Value &targetArray, rapidjson::MemoryPoolAllocator<> &alloc, const std::string &fieldname)
{
    const pqxx::row::size_type column(result[currentRow].column_number(fieldname));
    for (size_t i(0); i < size(); ++i)
    {
        rapidjson::Value data(string(column), alloc);
        targetArray.PushBack(data, alloc);
        next();
    }
    return targetArray.Size();
}
