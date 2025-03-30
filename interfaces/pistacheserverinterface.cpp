#include "pistacheserverinterface.h"
#include "logstat/logstatcontroller.h"

PistacheServerInterface::PistacheServerInterface(int port,
                                                 const std::string &certFilename,
                                                 const std::string &keyFilename):
    addr(Pistache::Ipv4::any(), Pistache::Port(port)),
    server(addr),
    certFilename(certFilename),
    keyFilename(keyFilename)
{

}

void PistacheServerInterface::serve()
{
    int threads(1);
    auto opts = Pistache::Http::Endpoint::options().threads(threads);
    opts.maxRequestSize(10 * 1024 * 1024);
    opts.maxResponseSize(10 * 1024 * 1024);
    opts.flags(Pistache::Tcp::Options::ReuseAddr);
    server.init(opts);
    if (certFilename.size() && keyFilename.size())
    {
        LogStatController::slog(__FILE__, __LINE__, LogStatController::info, "using ssl");
        server.useSSL(certFilename, keyFilename);
    }
    else
    {
        LogStatController::slog(__FILE__, __LINE__, LogStatController::info, "not using ssl");
    }

    server.setHandler(router.handler());

    server.serve();

}
