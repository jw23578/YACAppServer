#include "pistachehandlerinterface.h"
#include "extpistache.h"
#include "rapidjson/error/en.h"
#include "JWUtils/definitions.h"
#include "JWUtils/extstring.h"
#include "JWUtils/extuuid.h"
#include "beginendtrack.h"

void PistacheHandlerInterface::internalMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
{
    BeginEndTrack bet(__FILE__, __LINE__, request.resource());
    this->request = &request;
    this->response = &response;
    if (loginNeeded == TypeLoginNeeded && !checkLogin())
    {
        return;
    }
    if (request.method() == Pistache::Http::Method::Post
        || request.method() == Pistache::Http::Method::Put)
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
    MACRO_GetBool(prettyJson);
    ep.prettyJson = prettyJson;
    method();
    bet.track(__FILE__, __LINE__);
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

void PistacheHandlerInterface::answerOkHtml(const std::string &html)
{
    answerHtml(Pistache::Http::Code::Ok, html);
}

const rapidjson::Document &PistacheHandlerInterface::getPostedData() const
{
    return postedData;
}

bool PistacheHandlerInterface::isGet() const
{
    return request->method() == Pistache::Http::Method::Get;
}

bool PistacheHandlerInterface::isPost() const
{
    return request->method() == Pistache::Http::Method::Post;
}

bool PistacheHandlerInterface::isDelete() const
{
    return request->method() == Pistache::Http::Method::Delete;
}

bool PistacheHandlerInterface::isPut() const
{
    return request->method() == Pistache::Http::Method::Put;
}

bool PistacheHandlerInterface::isMethod(const std::string &method) const
{
    return request->resource() == method || request->resource() == "/" + method;
}

const std::string &PistacheHandlerInterface::requestRessource() const
{
    return request->resource();
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      const std::string &message,
                                      bool success,
                                      int missingRight)
{
    ep.answer(*response, code, message, success, missingRight);
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      bool success,
                                      int missingRight,
                                      rapidjson::Document &d)
{
    ep.answer(*response, code, success, missingRight, d);
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      const std::string &message,
                                      bool success,
                                      int missingRight,
                                      std::map<std::string, std::string> &data)
{
    MACRO_GetBool(prettyJson);
    ep.answer(*response, code, message, success, missingRight, data);
}

void PistacheHandlerInterface::answerHtml(Pistache::Http::Code code,
                                          const std::string &html)
{
    ep.answerRaw(*response, code, html, MIME(Text, Html));
}


bool PistacheHandlerInterface::answerMissingRight(const int rightNumber)
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

void PistacheHandlerInterface::addAllMethodTypes(PistacheServerInterface &serverInterface,
                                                 std::string const &methodName)
{
    addMethod(serverInterface, methodName, TypeGet);
    addMethod(serverInterface, methodName, TypePost);
    addMethod(serverInterface, methodName, TypeDelete);
    addMethod(serverInterface, methodName, TypePut);
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
    if (type == TypePut)
    {
        Pistache::Rest::Routes::Put(serverInterface.router, slash + methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
        return;
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
                                                   LoginNeededType loginNeeded):
    request(0), response(0),
    loginNeeded(loginNeeded)
{
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
    if (request->method() == Pistache::Http::Method::Get || request->method() == Pistache::Http::Method::Delete)
    {
        return ep.getBool(*request, *response, name, target, ifMissingThenSendResponse);
    }
    if (request->method() == Pistache::Http::Method::Post)
    {
        ExtRapidJSON postData(postedData);
        return ep.getPostBool(postData, *response, name, target, ifMissingThenSendResponse);
    }
    return false;
}

bool PistacheHandlerInterface::getInteger(const std::string &name,
                                          int &target,
                                          bool zeroAllowed,
                                          bool ifMissingThenSendResponse)
{
    if (request->method() == Pistache::Http::Method::Get || request->method() == Pistache::Http::Method::Delete)
    {
        ep.getInt(*request, *response, name, target, ifMissingThenSendResponse);
    }
    if (request->method() == Pistache::Http::Method::Post)
    {
        ExtRapidJSON postData(postedData);
        ep.getPostInt(postData, *response, name, target, ifMissingThenSendResponse);
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
    if (request->method() == Pistache::Http::Method::Get || request->method() == Pistache::Http::Method::Delete)
    {
        std::string temp;
        if (!ep.getString(*request, *response, name, temp, ifMissingThenSendResponse))
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
        if (!ep.getPostString(postData, *response, name, temp, ifMissingThenSendResponse))
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
    if (request->method() == Pistache::Http::Method::Get || request->method() == Pistache::Http::Method::Delete)
    {
        return ep.getString(*request, *response, name, target, ifMissingThenSendResponse);
    }
    if (request->method() == Pistache::Http::Method::Post)
    {
        ExtRapidJSON postData(postedData);
        return ep.getPostString(postData, *response, name, target, ifMissingThenSendResponse);
    }
    return true;
}

bool PistacheHandlerInterface::getUuid(const std::string &name, reducedsole::uuid &target, bool ifMissingThenSendResponse)
{
    std::string temp;
    if (!getString(name, temp, ifMissingThenSendResponse))
    {
        return false;
    }
    target = reducedsole::rebuild(temp);
    if (target == ExtUuid::NullUuid && ifMissingThenSendResponse)
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
    if (temp.find("1900-01-01T12:00:00") == 0)
    {
        tp = TimePointPostgreSqlNull;
        return true;
    }
    tp = ExtString::toTimepoint(temp);
    return true;
}

bool PistacheHandlerInterface::checkLogin()
{
    return true;
}

