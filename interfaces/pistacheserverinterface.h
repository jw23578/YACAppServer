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
    Pistache::Rest::Router router;
public:
    PistacheServerInterface(int port);
    void serve();
};

#endif // PISTACHESERVERINTERFACE_H
