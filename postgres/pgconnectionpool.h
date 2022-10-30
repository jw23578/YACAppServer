#ifndef PGCONNECTIONPOOLS_H
#define PGCONNECTIONPOOLS_H

#include <mutex>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "logstat/logstatcontroller.h"

class PGConnection;

class PGConnectionPool
{
    friend PGConnection;
    class PGConnectionContainer
    {
    public:
        bool inUse;
        pqxx::connection *conn;
        PGConnectionContainer(): inUse(false)
          , conn(0) {}
    };
    std::string host;
    int port;
    std::string  dbName, user, password;
    size_t maxSize;
    LogStatController &logStatController;
    int currentInUse;
    std::vector<PGConnectionContainer> connections;


    PGConnectionPool(const PGConnectionPool &other);// not implemented for not beeing used {}
    PGConnectionPool(); // not implemeneted for not beeing used {}

    std::mutex theMutex;
    bool findAndUse(pqxx::connection **conn);
    void releaseConnection(pqxx::connection* conn);
    pqxx::connection* getConnection();

public:
    PGConnectionPool(std::string const &h,
                     int p,
                     std::string const &db,
                     std::string const &u,
                     std::string const &pwd,
                     size_t ms,
                     LogStatController &logStatController);

    std::string generateConnstring() const;

    LogStatController &getLC();

    /*    pqxx::result execDirect(PGSqlString const &sql,
                            bool noTransaction,
                            const std::string &dbName,
                            const std::string &user,
                            const std::string &password,
                            const std::string &host,
                            int port) const; */

};


#endif // PGCONNECTIONPOOLS_H
