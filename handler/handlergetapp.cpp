#include "handlergetapp.h"

HandlerGetAPP::HandlerGetAPP(PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/getAPP",
                             TypeGet)
{

}

void HandlerGetAPP::method()
{

}
