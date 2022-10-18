#include "pgconnection.h"
#include "pgconnectionpool.h"

pqxx::connection *PGConnection::getConnection()
{
    return conn;
}

PGConnection::PGConnection(PGConnectionPool &pool):
    pool(pool), conn(pool.getConnection())
{

}

PGConnection::~PGConnection()
{
    if (!conn)
    {
        return;
    }
    pool.releaseConnection(conn);
}
