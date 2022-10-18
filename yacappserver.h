#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "pistache/router.h"
#include "pistache/endpoint.h"
#include "handler/handlerregister.h"
#include "interfaces/pistacheserverinterface.h"

class YACAppServer: public PistacheServerInterface
{
    HandlerRegister handlerRegister;

    void getAPPMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
public:
    YACAppServer(int port = 23578);
};

#endif // YACAPPSERVER_H
