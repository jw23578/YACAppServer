#include "pgconnectionpool.h"
#include "utils/extstring.h"

PGConnectionPool::PGConnectionPool()
{

}

bool PGConnectionPool::findAndUse(pqxx::connection **conn)
{
    for (size_t i(0); i < connections.size(); ++i)
    {
        PGConnectionContainer &c(connections[i]);
        if (!c.inUse)
        {
            c.inUse = true;
            *conn = c.conn;
            return true;
        }
    }
    return false;
}

void PGConnectionPool::releaseConnection(pqxx::connection *conn)
{
    std::lock_guard<std::mutex> lock(theMutex);
    for (size_t i(0); i < connections.size(); ++i)
    {
        PGConnectionContainer &c(connections[i]);
        if (c.conn == conn)
        {
            c.inUse = false;
            return;
        }
    }
}

PGConnectionPool::PGConnectionPool(const std::string &h,
                                   int p,
                                   const std::string &db,
                                   const std::string &u,
                                   const std::string &pwd,
                                   size_t ms):
    host(h),
    port(p),
    dbName(db),
    user(u),
    password(pwd),
    maxSize(ms)
{
}

std::string PGConnectionPool::generateConnstring() const
{
    std::stringstream connstring;
    connstring << std::string(" host=") << host;
    connstring << std::string(" port=") << port;
    connstring << std::string(" dbname=") << ExtString::lower(dbName);
    connstring << std::string(" user=") << ExtString::lower(user);
    connstring << std::string(" password=") << password;
    return connstring.str();
}


/*pqxx::result PGConnectionPools::execDirect(const PGSqlString &sql,
                                           bool noTransaction,
                                           const std::string &dbName,
                                           const std::string &user,
                                           const std::string &password,
                                           const std::string &host,
                                           int port) const
{
    std::cout << "sql: " << sql.str() << std::endl;
    std::string cs(generateConnstring(dbName,
                                      user,
                                      password,
                                      host,
                                      port));
    pqxx::connection c(cs.c_str());
    if (noTransaction)
    {
        pqxx::nontransaction w(c);
        pqxx::result result(w.exec(sql.str()));
        w.commit();
        return result;
    }
    pqxx::work w(c);
    pqxx::result result(w.exec(sql.str()));
    w.commit();
    return result;
}*/

pqxx::connection *PGConnectionPool::getConnection()
{
    std::lock_guard<std::mutex> lock(theMutex);
    pqxx::connection *conn(0);
    if (findAndUse(&conn))
    {
        return conn;
    }
    PGConnectionContainer c;
    c.inUse = true;
    std::string cs(generateConnstring());
    c.conn = new pqxx::connection(cs.c_str());
    connections.push_back(c);
    return c.conn;
}
