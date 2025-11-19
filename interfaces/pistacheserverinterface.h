#ifndef PISTACHESERVERINTERFACE_H
#define PISTACHESERVERINTERFACE_H

#include "pistache/router.h"
#include "pistache/endpoint.h"
#include "methodinfo.h"

class PistacheHandlerInterface;

class PistacheServerInterface
{
    friend PistacheHandlerInterface;
    const std::string &baseUrlWithoutPort;
    Pistache::Address addr;
    Pistache::Http::Endpoint server;
    const std::string certFilename;
    const std::string keyFilename;
    Pistache::Rest::Router router;
public:
    PistacheServerInterface(const std::string &baseUrlWithoutPort,
                            int port,
                            const std::string &certFilename,
                            const std::string &keyFilename);
    void serve();

    const std::string getBaseUrl() const;

    std::vector<MethodInfo> allMethods;
};

#endif // PISTACHESERVERINTERFACE_H
