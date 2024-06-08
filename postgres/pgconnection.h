#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include <pqxx/pqxx>

class PGConnectionPool;
class PGCommandTransactor;
class PGOidStorer;
class PGOidLoader;
class PGSqlImplementation;

class PGConnection
{
    friend PGCommandTransactor;
    friend PGOidStorer;
    friend PGOidLoader;
    friend PGSqlImplementation;
    PGConnectionPool &pool;
    pqxx::connection *conn;
    pqxx::connection *getConnection();
public:
    PGConnection(PGConnectionPool &pool);
    ~PGConnection();
};

#endif // PGCONNECTION_H
