#ifndef PGUTILS_H
#define PGUTILS_H

#include "pgconnectionpool.h"
#include "pgsqlstring.h"

enum PGTypes
{
    pg_int,
    pg_bigint,
    pg_text,
    pg_float,
    pg_bool,
    pg_timestamp,
    pg_uuid
};

class PGColumnAndType
{
  public:
    const std::string column;
    const PGTypes type;
    const bool primaryKey;
    const bool index;
    PGColumnAndType(const std::string &column, PGTypes type);
    PGColumnAndType(const std::string &column, PGTypes type, const bool primaryKey);
    PGColumnAndType(const std::string &column, PGTypes type, const bool primaryKey, const bool index);
    std::string toString() const;
    std::string primaryKeyString() const;
};

class PGUtils
{
    static std::map<std::string, PGSqlString> tableName2InsertString;
    static std::map<std::string, PGSqlString> tableName2UpdateString;
    PGConnectionPool &pool;
public:
    PGUtils(PGConnectionPool &pool);
    bool tableHasColumn(std::string const &tableName,
                        std::string const &columnName) const;
    void alterTableAddColumn(std::string const &tableName,
                             std::string const &columnName,
                             std::string const &columnType) const;
    void alterTableAddColumnIfNeeded(std::string const &tableName,
                                     std::string const &columnName,
                                     std::string const &columnType) const;
    bool createTable(const std::string &tableName,
                     const std::vector<PGColumnAndType> &columnsAndTypes);
    void createTableIfNeeded(const std::string &tableName,
                             const std::vector<PGColumnAndType> &columnsAndTypes);
    bool tableExists(std::string const &tableName) const;
    bool tableEmpty(std::string const &tableName) const;
    bool databaseExists(const std::string &databaseName) const;

    bool createRole(const std::string &name,
                    const std::string &password,
                    bool superUser) const;
    bool dropRole(const std::string &name) const;
    bool createDatabase(const std::string &databaseName,
                        const std::string &owner) const;

    bool pgCryptoInstalled() const;
    bool installPGCrypto() const;
    bool roleExists(const std::string &roleName) const;
    bool indexExists(const std::string &tableName,
                     const std::string &indexName) const;
    void createIndex(const std::string &tableName,
                     const std::string &indexName,
                     const std::string &definition) const;

    PGSqlString createEntryExistsString(const std::string &tableName,
                                        const std::string &needleField);
    bool entryExists(const std::string &tableName,
                     const std::string &needleField,
                     const std::string &needleValue);

    size_t countEntries(const std::string &tableName,
                        const std::string &needleField,
                        const std::string &needleValue);

    PGSqlString createInsertString(const std::string &tableName);
    PGSqlString createUpdateString(const std::string &tableName,
                                   const std::string &needleField);
};

#endif // PGUTILS_H
