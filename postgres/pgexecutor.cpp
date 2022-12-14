#include "pgexecutor.h"
#include "pgcommandtransactor.h"
#include "utils/extstring.h"
#include "definitions.h"


PGExecutor::PGExecutor(PGConnectionPool &pool):
    pool(pool)
{

}

PGExecutor::PGExecutor(PGConnectionPool &pool,
                       const PGSqlString &sql): pool(pool)
{
    PGCommandTransactor ct(pool, sql, result);
}

size_t PGExecutor::exec(const PGSqlString &sql)
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
    PGSqlString sql("select *, ");
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
    PGSqlString sql("select * from ");
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
    PGSqlString sql("select * from ");
    sql += tableName;
    sql.addCompare("where", needleField, "=", needleValue);
    sql.addCompare("and", needleField2, "=", needleValue2);
    sql.addCompare("and", needleField3, needleValue3 == TimePointPostgreSqlNull ? "is" : "=", needleValue3);
    return exec(sql);
}

void PGExecutor::erase(const std::string &tableName, const std::string &needleField, const std::string &needleValue)
{
    PGSqlString sql("delete from ");
    sql += tableName;
    sql += " where " + needleField;
    sql += " = :" + needleField;
    sql.set(needleField, needleValue);
    exec(sql);
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

std::string PGExecutor::string(const pqxx::row::size_type columnNumber)
{
    const pqxx::row &row(result[currentRow]);
    if (row[columnNumber].is_null())
    {
        return "";
    }
    return row[columnNumber].get<std::string>().value();
}

std::string PGExecutor::string(const std::string &fieldname)
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

void PGExecutor::toJsonObject(rapidjson::Value &object, rapidjson::MemoryPoolAllocator<> &alloc)
{
    const pqxx::row &row(result[currentRow]);
    for (pqxx::row::size_type i(0); i < row.size(); ++i)
    {
        rapidjson::Value name(row[i].name(), alloc);
        std::string data(string(i));
        object.AddMember(name, data, alloc);
    }
}

size_t PGExecutor::toJsonArray(rapidjson::Value &target, rapidjson::MemoryPoolAllocator<> &alloc)
{
    target.SetArray();
    for (size_t r(0); r < size(); ++r)
    {
        rapidjson::Value object(rapidjson::kObjectType);
        object.SetObject();
        toJsonObject(object, alloc);
        target.PushBack(object, alloc);
        next();
    }
    return size();

}
