#ifndef PGUTILS_H
#define PGUTILS_H

#include "pgconnectionpool.h"
#include "pgsqlstring.h"
#include "pgcolumnandtype.h"

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
    void alterTableAddColumnIfNeeded(std::string const &tableName,
                                     const PGColumnAndType &columnAndType) const;
    void alterTableAddColumnsIfNeeded(std::string const &tableName,
                                      const std::vector<PGColumnAndType> &columnsAndTypes) const;
    bool createTable(const std::string &tableName,
                     const std::vector<PGColumnAndType> &columnsAndTypes,
                     const std::vector<std::vector<std::string> > &uniques = std::vector<std::vector<std::string> >()) const;
    void createTableIfNeeded(const std::string &tableName,
                             const std::vector<PGColumnAndType> &columnsAndTypes,
                             const std::vector<std::vector<std::string> > &uniques = std::vector<std::vector<std::string> >()) const;
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
    void createUniqueIndex(const std::string &tableName,
                           const std::string &indexName,
                           const std::string &definition) const;

    PGSqlString createEntryExistsString(const std::string &tableName,
                                        const std::string &needleField);

    bool entryExists(const std::string &tableName,
                     const std::string &needleField,
                     const std::string &needleValue);

    bool entryExists(const std::string &tableName,
                     const std::string &needleField,
                     const std::string &needleValue,
                     const std::string &needleField2,
                     const std::string &needleValue2);

    bool entryExists(const std::string &tableName,
                     const std::string &needleField,
                     const std::string &needleValue,
                     const std::string &needleField2,
                     const std::string &needleValue2,
                     const std::string &needleField3,
                     const std::chrono::system_clock::time_point &needleValue3);

    size_t countEntries(const std::string &tableName,
                        const std::string &needleField,
                        const std::string &needleValue);

    PGSqlString createInsertString(const std::string &tableName);
    PGSqlString createUpdateString(const std::string &tableName,
                                   const std::string &needleField);
};

#endif // PGUTILS_H
