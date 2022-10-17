#include "yacappserver.h"
#include "pistache/router.h"

YACAppServer::YACAppServer(int port):
    addr(Pistache::Ipv4::any(), Pistache::Port(port)),
    server(addr),
    handlerRegister(router)

{
//    Pistache::Rest::Routes::Post(router, "/login", Pistache::Rest::Routes::bind(&YACAppServer::loginMethod, this));
//    Pistache::Rest::Routes::Post(router, "/uploadAPP", Pistache::Rest::Routes::bind(&YACAppServer::uploadAPPMethod, this));
//    Pistache::Rest::Routes::Get(router, "/getAPP", Pistache::Rest::Routes::bind(&YACAppServer::getAPPMethod, this));
    int threads(1);
    auto opts = Pistache::Http::Endpoint::options().threads(threads);
    opts.maxRequestSize(100000);
    opts.maxResponseSize(100000);
    opts.flags(Pistache::Tcp::Options::ReuseAddr);
    server.init(opts);
    //    server.useSSL()

    server.setHandler(router.handler());

    server.serve();
}
