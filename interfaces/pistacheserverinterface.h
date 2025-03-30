#ifndef PISTACHESERVERINTERFACE_H
#define PISTACHESERVERINTERFACE_H

#include "pistache/router.h"
#include "pistache/endpoint.h"

class PistacheHandlerInterface;

class PistacheServerInterface
{
    friend PistacheHandlerInterface;
    Pistache::Address addr;
    Pistache::Http::Endpoint server;
    const std::string certFilename;
    const std::string keyFilename;
    Pistache::Rest::Router router;
public:
    PistacheServerInterface(int port,
                            const std::string &certFilename,
                            const std::string &keyFilename);
    void serve();
};

#endif // PISTACHESERVERINTERFACE_H
