#ifndef PGUTILS_H
#define PGUTILS_H

#include "pgconnectionpool.h"
#include "pgsqlstring.h"

class PGUtils
{
    static std::map<std::string, PGSqlString> tableName2InsertString;
    static std::map<std::string, PGSqlString> tableName2UpdateString;
    PGConnectionPool &pool;
public:
    PGUtils(PGConnectionPool &pool);
    void alterTableAddColumn(std::string const &tableName,
                             std::string const &columnName,
                             std::string const &columnType) const;
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

    PGSqlString createInsertString(const std::string &tableName);
    PGSqlString createUpdateString(const std::string &tableName,
                                   const std::string &needleField);
};

#endif // PGUTILS_H
