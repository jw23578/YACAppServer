#ifndef PGOIDSTORER_H
#define PGOIDSTORER_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"

class PGOidStorer
{
    void exec(PGConnectionPool &pool,
              const std::basic_string<std::byte> &data,
              pqxx::oid &result);
public:
    PGOidStorer(PGConnectionPool &pool,
                const std::basic_string<std::byte> &data,
                pqxx::oid &result);
    PGOidStorer(PGConnectionPool &pool,
                const std::string &data,
                pqxx::oid &result);
};

#endif // PGOIDSTORER_H
