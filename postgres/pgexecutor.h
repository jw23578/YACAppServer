#ifndef PGEXECUTOR_H
#define PGEXECUTOR_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"
#include "orm_implementions/sqlstring.h"
#include "rapidjson/document.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm-mapper/orm2postgres.h"

#define MACRO_Uuid_NotEqual(executor, id) \
    if (executor.uuid(#id) != id)

class PGExecutor
{
    PGConnectionPool &pool;
    pqxx::result result;
    size_t currentRow = {0};
public:
    PGExecutor(PGConnectionPool &pool);
    PGExecutor(PGConnectionPool &pool,
               SqlString const &sql);
    PGExecutor(PGConnectionPool &pool,
               SqlString const &sql,
               std::string &resultMessage,
               const std::string &onSizeMessage,
               const std::string &onZeroSizeMessage);

    size_t exec(SqlString const &sql);

    size_t login(const std::string &tableName,
                 const std::string &passwordHashfield,
                 const std::string &password,
                 const std::string &loginField,
                 const std::string &loginValue);

    template <class NV1>
    size_t select(const std::string &tableName,
                  const std::string &needleField,
                  const NV1 &needleValue)
    {
        SqlString sql("select * from ");
        sql += tableName;
        sql += " where " + needleField;
        sql += " = :" + needleField;
        sql.set(needleField, needleValue);
        return exec(sql);
    }

    size_t select(const std::string &tableName,
                  const std::string &needleField,
                  const std::string &needleValue,
                  const std::string &needleField2,
                  const std::string &needleValue2);

    size_t select(const std::string &tableName,
                  const std::string &needleField,
                  const std::string &needleValue,
                  const std::string &needleField2,
                  const std::string &needleValue2,
                  const std::string &needleField3,
                  const std::chrono::system_clock::time_point &needleValue3);

    bool defaultSelect(const std::string &tableName,
                       const sole::uuid &id,
                       std::string &message);

    void delet(const std::string &tableName,
               const std::string &needleField,
               const std::string &needleValue);

    void defaultDelete(const std::string &table_name,
                       const sole::uuid &id);

    size_t size() const;
    bool resultAvailable() const;
    size_t affected_rows() const;
    size_t columns();

    size_t next();

    std::string columnName(size_t c);

    bool isNull(const std::string &fieldname) const;
    bool isNullOrEmpty(const std::string &fieldname);
    bool boolean(const std::string &fieldname);
    std::string string(const pqxx::row::size_type columnNumber) const;
    std::string string(const std::string &fieldname) const;
    int integer(const std::string &fieldname);
    size_t get_size_t(const std::string &fieldname);
    std::chrono::system_clock::time_point timepoint(const std::string &fieldname);
    sole::uuid uuid(const std::string &fieldname);
    pqxx::oid oid(const std::string &fieldname);

    size_t array(const std::string &fieldname, std::set<std::string> &target);

    size_t fill(std::set<int> &target, const std::string &fieldname);
    size_t append(std::set<int> &target, const std::string &fieldname);

    size_t fill(rapidjson::Value &targetArray, rapidjson::MemoryPoolAllocator<> &alloc, const std::string &fieldname);
    size_t append(rapidjson::Value &targetArray, rapidjson::MemoryPoolAllocator<> &alloc, const std::string &fieldname);
};

#endif // PGEXECUTOR_H
