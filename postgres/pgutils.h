#ifndef PGUTILS_H
#define PGUTILS_H

#include "pgconnectionpool.h"

class PGUtils
{
public:
    PGUtils();
    void alterTableAddColumn(std::string const &tableName,
                             std::string const &columnName,
                             std::string const &columnType,
                             PGConnectionPool &pool) const;
    bool tableExists(std::string const &tableName,
                     PGConnectionPool &pool) const;
    bool tableEmpty(std::string const &tableName,
                    PGConnectionPool &pool) const;
    bool databaseExists(const std::string &databaseName,
                        PGConnectionPool &pool) const;

    bool createRole(const std::string &name,
                    const std::string &password,
                    bool superUser,
                    PGConnectionPool &pool) const;
    bool dropRole(const std::string &name,
                  PGConnectionPool &pool) const;
    bool createDatabase(const std::string &databaseName,
                        const std::string &owner,
                        PGConnectionPool &pool) const;

    bool pgCryptoInstalled(std::string const dbName,
                           PGConnectionPool &pool) const;
    bool installPGCrypto(const std::string &dbName,
                         PGConnectionPool &pool) const;
    bool roleExists(const std::string &roleName,
                    PGConnectionPool &pool) const;
};

#endif // PGUTILS_H
