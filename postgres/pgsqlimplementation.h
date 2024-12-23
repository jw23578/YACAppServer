#ifndef PGSQLIMPLEMENTATION_H
#define PGSQLIMPLEMENTATION_H

#include <orm/ormsqlinterface.h>
#include "pgconnectionpool.h"
#include "pqxx/pqxx"

class PGSqlImplementation : public ORMSqlInterface
{
    PGConnectionPool &pool;
    pqxx::result result;
public:
    PGSqlImplementation(PGConnectionPool &pool);
    virtual ~PGSqlImplementation() {}


protected:
    bool internalExecute(const SqlString &sql) override;
    bool internalOpen(SqlString const &sql) override;
    // ORMSqlInterface interface
public:    
    size_t size() override;
    bool next() override;
    size_t storeBlob(const std::basic_string<std::byte> &data) override;
    bool fetchBlob(size_t blobId, std::basic_string<std::byte> &data) override;
    size_t columns() override;
    std::string columnName(size_t index) override;
    std::optional<ORMUuid> uuidValue(size_t index) override;
    std::optional<std::string> value(size_t index) override;
    std::optional<int> intValue(size_t index) override;
    std::optional<std::chrono::system_clock::time_point> timepointValue(size_t index) override;
    bool isNull(size_t index) override;
};

#endif // PGSQLIMPLEMENTATION_H
