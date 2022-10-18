#include "handlerregister.h"

HandlerRegister::HandlerRegister(PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/register",
                             TypePost)
{

}

void HandlerRegister::method()
{
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("name", rapidjson::StringRef("Jens"), d.GetAllocator());
    answer(Pistache::Http::Code::Ok, d);
}
