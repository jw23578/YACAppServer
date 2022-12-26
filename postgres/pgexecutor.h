#ifndef PGEXECUTOR_H
#define PGEXECUTOR_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"
#include "pgsqlstring.h"
#include "rapidjson/document.h"

class PGExecutor
{
    PGConnectionPool &pool;
    pqxx::result result;
    size_t currentRow = {0};
public:
    PGExecutor(PGConnectionPool &pool);
    PGExecutor(PGConnectionPool &pool,
               PGSqlString const &sql);

    size_t exec(PGSqlString const &sql);

    size_t login(const std::string &tableName,
                 const std::string &passwordHashfield,
                 const std::string &password,
                 const std::string &loginField,
                 const std::string &loginValue);

    size_t select(const std::string &tableName,
                  const std::string &needleField,
                  const std::string &needleValue);

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

    void erase(const std::string &tableName,
               const std::string &needleField,
               const std::string &needleValue);

    size_t size();
    size_t columns();

    size_t next();

    std::string columnName(size_t c);

    bool isNull(const std::string &fieldname);
    bool isNullOrEmpty(const std::string &fieldname);
    bool boolean(const std::string &fieldname);
    std::string string(const pqxx::row::size_type columnNumber);
    std::string string(const std::string &fieldname);
    int integer(const std::string &fieldname);
    size_t get_size_t(const std::string &fieldname);
    std::chrono::system_clock::time_point timepoint(const std::string &fieldname);
    sole::uuid uuid(const std::string &fieldname);
    pqxx::oid oid(const std::string &fieldname);

    size_t toJsonArray(rapidjson::Value &target, rapidjson::MemoryPoolAllocator<> &alloc);
    void toJsonObject(rapidjson::Value &object, rapidjson::MemoryPoolAllocator<> &alloc);
};

#endif // PGEXECUTOR_H
