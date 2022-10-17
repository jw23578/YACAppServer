#include "pistachehandlerinterface.h"
#include "rapidjson/writer.h"

void PistacheHandlerInterface::internalMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
{
    this->request = &request;
    this->response = &response;
    method();
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      rapidjson::Document &d)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    buffer.Put('\n');
    response->send(code, buffer.GetString(), MIME(Application, Json));
}



PistacheHandlerInterface::PistacheHandlerInterface(Pistache::Rest::Router &router,
                                                   std::string const &methodName,
                                                   HandlerType type):request(0), response(0)
{
    if (type == TypeDelete)
    {
        Pistache::Rest::Routes::Delete(router, methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
        return;
    }
    if (type == TypePost)
    {
        Pistache::Rest::Routes::Post(router, methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
        return;
    }
    if (type == TypeGet)
    {
        Pistache::Rest::Routes::Get(router, methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
    }
}
