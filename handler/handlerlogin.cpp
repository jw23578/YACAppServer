#include "handlerlogin.h"

HandlerLogin::HandlerLogin(Pistache::Rest::Router &router):
    PistacheHandlerInterface(router,
                             "/login",
                             TypePost)
{

}

void HandlerLogin::method()
{

}
