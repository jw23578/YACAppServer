#ifndef PGOIDLOADER_H
#define PGOIDLOADER_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"

class PGOidLoader
{
public:
    PGOidLoader(PGConnectionPool &pool,
                const pqxx::oid &oid,
                std::string &target);
};

#endif // PGOIDLOADER_H
