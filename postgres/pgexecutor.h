#ifndef PGEXECUTOR_H
#define PGEXECUTOR_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"
#include "pgsqlstring.h"

class PGExecutor
{
    PGConnectionPool &pool;
    pqxx::result result;
    size_t currentRow = {0};
public:
    PGExecutor(PGConnectionPool &pool,
               PGSqlString const &sql);

    size_t exec(PGSqlString const &sql);

    size_t size();
    size_t columns();

    std::string columnName(size_t c);

    bool isNull(const std::string &fieldname);
    bool isNullOrEmpty(const std::string &fieldname);
    std::string string(const std::string &fieldname);
    std::chrono::system_clock::time_point timepoint(const std::string &fieldname);
    sole::uuid uuid(const std::string &fieldname);
};

#endif // PGEXECUTOR_H
