#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "pistache/router.h"
#include "pistache/endpoint.h"
#include "handler/handlerregister.h"

class YACAppServer
{
    Pistache::Address addr;
    Pistache::Http::Endpoint server;
    Pistache::Rest::Router router;

    HandlerRegister handlerRegister;

    void loginMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void uploadAPPMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void getAPPMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
public:
    YACAppServer(int port = 23578);
};

#endif // YACAPPSERVER_H
