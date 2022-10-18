#include "yacappserver.h"
#include "pistache/router.h"

YACAppServer::YACAppServer(int port):PistacheServerInterface(port),
    handlerRegister(*this)

{
//    Pistache::Rest::Routes::Post(router, "/login", Pistache::Rest::Routes::bind(&YACAppServer::loginMethod, this));
//    Pistache::Rest::Routes::Post(router, "/uploadAPP", Pistache::Rest::Routes::bind(&YACAppServer::uploadAPPMethod, this));
//    Pistache::Rest::Routes::Get(router, "/getAPP", Pistache::Rest::Routes::bind(&YACAppServer::getAPPMethod, this));
    serve();
}
