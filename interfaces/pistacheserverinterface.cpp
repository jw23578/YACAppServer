#include "pistacheserverinterface.h"

PistacheServerInterface::PistacheServerInterface(int port):
    addr(Pistache::Ipv4::any(), Pistache::Port(port)),
    server(addr)
{

}

void PistacheServerInterface::serve()
{
    int threads(1);
    auto opts = Pistache::Http::Endpoint::options().threads(threads);
    opts.maxRequestSize(1000000);
    opts.maxResponseSize(1000000);
    opts.flags(Pistache::Tcp::Options::ReuseAddr);
    server.init(opts);
    //    server.useSSL()

    server.setHandler(router.handler());

    server.serve();

}
