#include "handleruploadapp.h"

HandlerUploadAPP::HandlerUploadAPP(PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/uploadAPP",
                             TypePost)
{

}

void HandlerUploadAPP::method()
{

}
