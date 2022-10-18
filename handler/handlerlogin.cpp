#include "handlerlogin.h"

HandlerLogin::HandlerLogin(PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/login",
                             TypePost)
{

}

void HandlerLogin::method()
{

}
