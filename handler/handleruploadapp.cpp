#include "handleruploadapp.h"

HandlerUploadAPP::HandlerUploadAPP(PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/uploadAPP",
                             TypePost,
                             TypeLoginNeeded)
{

}

void HandlerUploadAPP::method()
{

}
