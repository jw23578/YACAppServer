#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "pistache/router.h"
#include "pistache/endpoint.h"
#include "handler/handlerregister.h"
#include "interfaces/pistacheserverinterface.h"
#include "postgres/pgconnectionpool.h"

class YACAppServer: public PistacheServerInterface
{
    PGConnectionPool postgresConnectionPool;
    HandlerRegister handlerRegister;

    void getAPPMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
public:
    YACAppServer(std::string const &postgresHost,
                 int postgresPort,
                 std::string const &postgresDBName,
                 std::string const &postgresUser,
                 std::string const &postgresPassword,
                 int port);
};

#endif // YACAPPSERVER_H
