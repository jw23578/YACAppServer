#ifndef PGEXECUTOR_H
#define PGEXECUTOR_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"
#include "pgsqlstring.h"

class PGExecutor
{
    pqxx::result result;
    size_t currentRow = {0};
public:
    PGExecutor(PGConnectionPool &pool,
               PGSqlString const &sql);

    size_t size();

    std::string string(const std::string &fieldname);
    std::chrono::system_clock::time_point timepoint(const std::string &fieldname);
};

#endif // PGEXECUTOR_H
