#include "pistachehandlerinterface.h"
#include "extpistache.h"
#include "rapidjson/error/en.h"
#include "definitions.h"
#include "extstring.h"

void PistacheHandlerInterface::internalMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
{
    this->request = &request;
    this->response = &response;
    if (loginNeeded == TypeLoginNeeded && !checkLogin())
    {
        return;
    }
    if (request.method() == Pistache::Http::Method::Post)
    {
        postedData.Parse(request.body().c_str());
        if (postedData.HasParseError())
        {
            std::map<std::string, std::string> answerData;
            answerData["error"] = rapidjson::GetParseError_En(postedData.GetParseError());
            answerBad("data is not valid json", answerData);
            return;
        }
    }
    method();
}

void PistacheHandlerInterface::answerBad(const std::string &message)
{
    const int missingRight(0);
    answer(Pistache::Http::Code::Bad_Request, message, false, missingRight);
}

void PistacheHandlerInterface::answerBad(const std::string &message,
                                         std::map<std::string, std::string> &data)
{
    const int missingRight(0);
    answer(Pistache::Http::Code::Bad_Request, message, false, missingRight, data);
}

void PistacheHandlerInterface::answerOk(const std::string &message,
                                        bool success)
{
    const int missingRight(0);
    answer(Pistache::Http::Code::Ok, message, success, missingRight);
}

void PistacheHandlerInterface::answerOk(const std::string &message,
                                        bool success,
                                        std::map<std::string, std::string> &data)
{
    const int missingRight(0);
    answer(Pistache::Http::Code::Ok, message, success, missingRight, data);
}

void PistacheHandlerInterface::answerOk(bool success,
                                        rapidjson::Document &d)
{
    const int missingRight(0);
    answer(Pistache::Http::Code::Ok, success, missingRight, d);
}

bool PistacheHandlerInterface::isMethod(const std::string &method) const
{
    return request->resource() == method || request->resource() == "/" + method;
}

const std::string &PistacheHandlerInterface::d_getMethodName() const
{
    return request->resource();
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      const std::string &message,
                                      bool success,
                                      int missingRight)
{
    ExtPistache::answer(*response, code, message, success, missingRight);
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      bool success,
                                      int missingRight,
                                      rapidjson::Document &d)
{
    ExtPistache::answer(*response, code, success, missingRight, d);
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      const std::string &message,
                                      bool success,
                                      int missingRight,
                                      std::map<std::string, std::string> &data)
{
    ExtPistache::answer(*response, code, message, success, missingRight, data);
}

bool PistacheHandlerInterface::missingRight(const int rightNumber)
{
    if (!rightNumber)
    {
        return false;
    }
    std::map<std::string, std::string> data;
    bool success(false);
    answer(Pistache::Http::Code::Ok, "missing right", success, rightNumber, data);
    return true;
}

void PistacheHandlerInterface::addMethod(PistacheServerInterface &serverInterface,
                                         std::string const &methodName,
                                         HandlerType type)
{
    if (!methodName.size())
    {
        return;
    }
    std::string slash;
    if (methodName[0] != '/')
    {
        slash = "/";
    }
    if (type == TypeDelete)
    {
        Pistache::Rest::Routes::Delete(serverInterface.router, slash + methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
        return;
    }
    if (type == TypePost)
    {
        Pistache::Rest::Routes::Post(serverInterface.router, slash + methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
        return;
    }
    if (type == TypeGet)
    {
        Pistache::Rest::Routes::Get(serverInterface.router, slash + methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
    }
}

PistacheHandlerInterface::PistacheHandlerInterface(PistacheServerInterface &serverInterface,
                                                   std::string const &methodName,
                                                   HandlerType type,
                                                   LoginNeededType loginNeeded):
    request(0), response(0),
    loginNeeded(loginNeeded)
{
    addMethod(serverInterface,
              methodName,
              type);
}

const rapidjson::Value &PistacheHandlerInterface::getPostedJsonValue(const std::string &name)
{
    if (!postedData.HasMember(name))
    {
        return emptyValue;
    }
    return postedData[name];
}

bool PistacheHandlerInterface::getBool(const std::string &name,
                                       bool &target,
                                       bool ifMissingThenSendResponse)
{
    if (request->method() == Pistache::Http::Method::Get)
    {
        return ExtPistache::getBool(*request, *response, name, target, ifMissingThenSendResponse);
    }
    if (request->method() == Pistache::Http::Method::Post)
    {
        ExtRapidJSON postData(postedData);
        return ExtPistache::getPostBool(postData, *response, name, target, ifMissingThenSendResponse);
    }
    return false;
}

bool PistacheHandlerInterface::getInteger(const std::string &name,
                                          int &target,
                                          bool zeroAllowed,
                                          bool ifMissingThenSendResponse)
{
    if (request->method() == Pistache::Http::Method::Get)
    {
        ExtPistache::getInt(*request, *response, name, target, ifMissingThenSendResponse);
    }
    if (request->method() == Pistache::Http::Method::Post)
    {
        ExtRapidJSON postData(postedData);
        ExtPistache::getPostInt(postData, *response, name, target, ifMissingThenSendResponse);
    }
    if (target == 0 && !zeroAllowed)
    {
        answerBad(name + std::string(" must be != 0"));
        return false;
    }
    return true;
}

bool PistacheHandlerInterface::getByteString(const std::string &name,
                                             std::basic_string<std::byte> &target,
                                             bool ifMissingThenSendResponse)
{
    if (request->method() == Pistache::Http::Method::Get)
    {
        std::string temp;
        if (!ExtPistache::getString(*request, *response, name, temp, ifMissingThenSendResponse))
        {
            return false;
        }
        target = std::basic_string<std::byte>(reinterpret_cast<std::byte*>(temp.data()), temp.size());
        return true;
    }
    if (request->method() == Pistache::Http::Method::Post)
    {
        ExtRapidJSON postData(postedData);
        std::string temp;
        if (!ExtPistache::getPostString(postData, *response, name, temp, ifMissingThenSendResponse))
        {
            return false;
        }
        target = std::basic_string<std::byte>(reinterpret_cast<std::byte*>(temp.data()), temp.size());
        return true;
    }
    return true;
}

bool PistacheHandlerInterface::getString(const std::string &name,
                                         std::string &target,
                                         bool ifMissingThenSendResponse)
{
    if (request->method() == Pistache::Http::Method::Get)
    {
        return ExtPistache::getString(*request, *response, name, target, ifMissingThenSendResponse);
    }
    if (request->method() == Pistache::Http::Method::Post)
    {
        ExtRapidJSON postData(postedData);
        return ExtPistache::getPostString(postData, *response, name, target, ifMissingThenSendResponse);
    }
    return true;
}

bool PistacheHandlerInterface::getUuid(const std::string &name, sole::uuid &target, bool ifMissingThenSendResponse)
{
    std::string temp;
    if (!getString(name, temp, ifMissingThenSendResponse))
    {
        return false;
    }
    target = sole::rebuild(temp);
    if (target == NullUuid && ifMissingThenSendResponse)
    {
        answerBad(std::string("Missing ") + name);
        return false;
    }
    return true;
}

bool PistacheHandlerInterface::getTimePointFromISO(const std::string &name,
                                                   std::chrono::system_clock::time_point &tp,
                                                   bool ifMissingThenSendResponse)
{
    std::string temp;
    if (!getString(name, temp, ifMissingThenSendResponse))
    {
        return false;
    }
    tp = ExtString::toTimepoint(temp);
    return true;
}

bool PistacheHandlerInterface::checkLogin()
{
    return true;
}

