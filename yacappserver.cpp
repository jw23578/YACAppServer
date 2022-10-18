#include "yacappserver.h"
#include "pistache/router.h"

YACAppServer::YACAppServer(std::string const &postgresHost,
                           int postgresPort,
                           std::string const &postgresDBName,
                           std::string const &postgresUser,
                           std::string const &postgresPassword,
                           int port):
    PistacheServerInterface(port),
    postgresConnectionPool(postgresHost,
                           postgresPort,
                           postgresDBName,
                           postgresUser,
                           postgresPassword,
                           10),
    handlerRegister(*this)

{
//    Pistache::Rest::Routes::Post(router, "/login", Pistache::Rest::Routes::bind(&YACAppServer::loginMethod, this));
//    Pistache::Rest::Routes::Post(router, "/uploadAPP", Pistache::Rest::Routes::bind(&YACAppServer::uploadAPPMethod, this));
//    Pistache::Rest::Routes::Get(router, "/getAPP", Pistache::Rest::Routes::bind(&YACAppServer::getAPPMethod, this));
    serve();
}
