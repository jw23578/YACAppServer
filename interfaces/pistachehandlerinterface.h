#ifndef PISTACHEHANDLERINTERFACE_H
#define PISTACHEHANDLERINTERFACE_H

#include "sole/sole.hpp"
#include "pistache/router.h"
#include "rapidjson/document.h"
#include "pistacheserverinterface.h"

#define MACRO_GetMandatoryByteString(targetName) std::basic_string<std::byte> targetName; \
    if (!getByteString(#targetName, targetName, true) || !targetName.size()) \
{ \
    return; \
    }

#define MACRO_GetMandatoryString(targetName) std::string targetName; \
    if (!getString(#targetName, targetName, true) || !targetName.size()) \
{ \
    return; \
    }

#define MACRO_GetMandatoryEMail(targetName) MACRO_GetMandatoryString(targetName) \
    if (!ExtString::emailIsValid(targetName)) \
{ \
    answerBad("this is not a valid email-adress: " + targetName); \
    return; \
    }

#define MACRO_GetMandatoryUuid(targetName) sole::uuid targetName; \
    if (!getUuid(#targetName, targetName, true)) \
{ \
    return; \
    }

#define MACRO_GetMandatoryBool(targetName) bool targetName(false); \
    if (!getBool(#targetName, targetName, true)) \
{ \
    return; \
    }

#define MACRO_GetString(targetName) std::string targetName; \
    getString(#targetName, targetName, false);

#define MACRO_GetBool(targetName) bool targetName(false); \
    getBool(#targetName, targetName, false);

#define MACRO_GetInt(targetName) int targetName(0); \
    getInteger(#targetName, targetName, true, false);

#define MACRO_GetMandatoryInt(targetName, zeroAllowed) int targetName(0); \
    if (!getInteger(#targetName, targetName, zeroAllowed, true)) \
{ \
    return; \
    }

#define MACRO_GetMandatoryTimePointFromISO(targetName) std::chrono::system_clock::time_point targetName; \
    if (!getTimePointFromISO(#targetName, targetName, true)) \
{ \
    return; \
    }

class PistacheHandlerInterface
{
    Pistache::Rest::Request const *request;
    Pistache::Http::ResponseWriter *response;
    rapidjson::Document postedData;
    const rapidjson::Value emptyValue;
    void internalMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void answer(Pistache::Http::Code code,
                const std::string &message,
                bool success);
    void answer(Pistache::Http::Code code,
                bool success,
                rapidjson::Document &d);
    void answer(Pistache::Http::Code code,
                const std::string &message,
                bool success,
                std::map<std::string, std::string> &data);

protected:
    void answerBad(const std::string &message);
    void answerBad(const std::string &message,
                   std::map<std::string, std::string> &data);
    void answerOk(const std::string &message,
                  bool success);
    void answerOk(const std::string &message,
                  bool success,
                  std::map<std::string, std::string> &data);
    void answerOk(bool success,
                  rapidjson::Document &d);
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

    const rapidjson::Value &getPostedJsonValue(std::string const &name);

    bool getBool(std::string const &name,
                 bool &target,
                 bool ifMissingThenSendResponse);

    bool getInteger(std::string const &name,
                    int &target,
                    bool zeroAllowed,
                    bool ifMissingThenSendResponse);

    bool getByteString(std::string const &name,
                       std::basic_string<std::byte> &target,
                       bool ifMissingThenSendResponse);

    bool getString(std::string const &name,
                   std::string &target,
                   bool ifMissingThenSendResponse);

    bool getUuid(std::string const &name,
                 sole::uuid &target,
                 bool ifMissingThenSendResponse);

    bool getTimePointFromISO(std::string const &name,
                             std::chrono::system_clock::time_point &tp,
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
                answer(Pistache::Http::Code::Bad_Request, std::string("Missing Header ") + T().name(), false);
            }
            return false;
        }
        target = headers.get<T>()->value;
        if (!target.size())
        {
            if (ifMissingThenSendResponse)
            {
                answer(Pistache::Http::Code::Bad_Request, std::string("Missing ") + T().name(), false);
            }
            return false;
        }
        return true;
    }
};

#endif // PISTACHEHANDLERINTERFACE_H
