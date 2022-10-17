#include "handleruploadapp.h"

HandlerUploadAPP::HandlerUploadAPP(Pistache::Rest::Router &router):
    PistacheHandlerInterface(router,
                             "/uploadAPP",
                             TypePost)
{

}

void HandlerUploadAPP::method()
{

}
