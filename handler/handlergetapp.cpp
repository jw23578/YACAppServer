#include "handlergetapp.h"

HandlerGetAPP::HandlerGetAPP(Pistache::Rest::Router &router):
    PistacheHandlerInterface(router,
                             "/getAPP",
                             TypeGet)
{

}

void HandlerGetAPP::method()
{

}
