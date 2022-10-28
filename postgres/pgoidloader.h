#ifndef PGOIDLOADER_H
#define PGOIDLOADER_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"

class PGOidLoader
{
public:
    PGOidLoader(PGConnectionPool &pool,
                const pqxx::oid &oid,
                std::basic_string<std::byte> &target);
};

#endif // PGOIDLOADER_H
