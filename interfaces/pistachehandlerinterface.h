#ifndef PISTACHEHANDLERINTERFACE_H
#define PISTACHEHANDLERINTERFACE_H

#include "pistache/router.h"
#include "rapidjson/document.h"
#include "pistacheserverinterface.h"

#define MACRO_GetMandatoryPostString(targetName) std::string targetName; \
    if (!getPostString(#targetName, targetName, true) || !targetName.size()) \
{ \
    return; \
}

class PistacheHandlerInterface
{
    Pistache::Rest::Request const *request;
    Pistache::Http::ResponseWriter *response;
    rapidjson::Document postedData;
    void internalMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
protected:
    void answer(Pistache::Http::Code code, const std::string &message);
    void answer(Pistache::Http::Code code, rapidjson::Document &d);
    void answer(Pistache::Http::Code code,
                const std::string &message,
                std::map<std::string, std::string> &data);
public:
    enum HandlerType
    {
        TypeGet,
        TypePost,
        TypeDelete
    };
    HandlerType handlerType;
    PistacheHandlerInterface(PistacheServerInterface &serverInterface,
                             const std::string &methodName,
                             HandlerType type);

    bool getString(std::string const &name,
                   std::string &target,
                   bool ifMissingThenSendResponse);
    bool getPostString(const std::string &name,
                       std::string &target,
                       bool ifMissingThenSendResponse);
    virtual void method() = 0;
};

#endif // PISTACHEHANDLERINTERFACE_H
