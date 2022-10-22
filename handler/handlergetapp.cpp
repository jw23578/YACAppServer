#include "handlergetapp.h"

HandlerGetAPP::HandlerGetAPP(PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/getAPP",
                             TypeGet,
                             TypeNoLoginNeeded)
{

}

void HandlerGetAPP::method()
{

}
