#ifndef PISTACHEHANDLERINTERFACE_H
#define PISTACHEHANDLERINTERFACE_H

#include "pistache/router.h"
#include "rapidjson/document.h"

class PistacheHandlerInterface
{
    Pistache::Rest::Request const *request;
    Pistache::Http::ResponseWriter *response;
    void internalMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
protected:
    void answer(Pistache::Http::Code code, rapidjson::Document &d);
public:
    enum HandlerType
    {
        TypeGet,
        TypePost,
        TypeDelete
    };
    PistacheHandlerInterface(Pistache::Rest::Router &router,
                             const std::string &methodName,
                             HandlerType type);

    virtual void method() = 0;
};

#endif // PISTACHEHANDLERINTERFACE_H
