#ifndef PISTACHEHANDLERINTERFACE_H
#define PISTACHEHANDLERINTERFACE_H

#include "pistache/router.h"
#include "rapidjson/document.h"
#include "pistacheserverinterface.h"

#define MACRO_GetMandatoryString(targetName) std::string targetName; \
    if (handlerType == TypeGet) \
    { \
        if (!getString(#targetName, targetName, true) || !targetName.size()) \
        { \
            return; \
        } \
    } \
    else \
    { \
        if (!getPostString(#targetName, targetName, true) || !targetName.size()) \
        { \
            return; \
        } \
    }

#define MACRO_GetMandatoryBool(targetName) bool targetName; \
    if (!getBool(#targetName, targetName, true)) \
    { \
        return; \
    }


#define MACRO_GetMandatoryInt(targetName, zeroAllowed) int targetName; \
    if (!getInteger(#targetName, targetName, zeroAllowed, true)) \
    { \
        return; \
    }

#define MACRO_GetMandatoryPostString(targetName) std::string targetName; \
    if (!getPostString(#targetName, targetName, true) || !targetName.size()) \
{ \
    return; \
}

#define MACRO_GetMandatoryGetString(targetName) std::string targetName; \
    if (!getString(#targetName, targetName, true) || !targetName.size()) \
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

    enum LoginNeededType
    {
        TypeNoLoginNeeded,
        TypeLoginNeeded
    };
    LoginNeededType loginNeeded;

    PistacheHandlerInterface(PistacheServerInterface &serverInterface,
                             const std::string &methodName,
                             HandlerType type,
                             LoginNeededType loginNeeded);

    bool getBool(std::string const &name,
                 bool &target,
                 bool ifMissingThenSendResponse);

    bool getInteger(std::string const &name,
                    int &target,
                    bool zeroAllowed,
                    bool ifMissingThenSendResponse);

    bool getString(std::string const &name,
                   std::string &target,
                   bool ifMissingThenSendResponse);
    bool getPostString(const std::string &name,
                       std::string &target,
                       bool ifMissingThenSendResponse);
    virtual void method() = 0;
    virtual bool checkLogin();

    template<class T>
    bool getHeaderString(std::string &target,
                                bool ifMissingThenSendResponse)
    {
        auto &headers(request->headers());
        if (!headers.has<T>())
        {
            if (ifMissingThenSendResponse)
            {
                answer(Pistache::Http::Code::Bad_Request, std::string("Missing Header ") + T().name());
            }
            return false;
        }
        target = headers.get<T>()->value;
        if (!target.size())
        {
            if (ifMissingThenSendResponse)
            {
                answer(Pistache::Http::Code::Bad_Request, std::string("Missing ") + T().name());
            }
            return false;
        }
        return true;
    }
};

#endif // PISTACHEHANDLERINTERFACE_H
