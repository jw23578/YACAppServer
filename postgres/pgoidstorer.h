#ifndef PGOIDSTORER_H
#define PGOIDSTORER_H

#include "pqxx/pqxx"
#include "pgconnectionpool.h"

class PGOidStorer
{
public:
    PGOidStorer(PGConnectionPool &pool,
                std::string const &data,
                pqxx::oid &result);
};

#endif // PGOIDSTORER_H
