#include "pistachehandlerinterface.h"
#include "extpistache.h"
#include "rapidjson/error/en.h"

void PistacheHandlerInterface::internalMethod(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
{
    this->request = &request;
    this->response = &response;
    if (loginNeeded == TypeLoginNeeded && !checkLogin())
    {
        return;
    }
    if (handlerType == TypePost)
    {
        postedData.Parse(request.body().c_str());
        if (postedData.HasParseError())
        {
            std::map<std::string, std::string> answerData;
            answerData["error"] = rapidjson::GetParseError_En(postedData.GetParseError());
            answer(Pistache::Http::Code::Bad_Request, "data is not valid json", answerData);
            return;
        }
    }
    method();
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      const std::string &message)
{
    ExtPistache::answer(*response, code, message);
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      rapidjson::Document &d)
{
    ExtPistache::answer(*response, code, d);
}

void PistacheHandlerInterface::answer(Pistache::Http::Code code,
                                      const std::string &message,
                                      std::map<std::string, std::string> &data)
{
    ExtPistache::answer(*response, code, message, data);
}

PistacheHandlerInterface::PistacheHandlerInterface(PistacheServerInterface &serverInterface,
                                                   std::string const &methodName,
                                                   HandlerType type,
                                                   LoginNeededType loginNeeded):request(0), response(0), handlerType(type),
    loginNeeded(loginNeeded)
{
    if (type == TypeDelete)
    {
        Pistache::Rest::Routes::Delete(serverInterface.router, methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
        return;
    }
    if (type == TypePost)
    {
        Pistache::Rest::Routes::Post(serverInterface.router, methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
        return;
    }
    if (type == TypeGet)
    {
        Pistache::Rest::Routes::Get(serverInterface.router, methodName, Pistache::Rest::Routes::bind(&PistacheHandlerInterface::internalMethod, this));
    }
}

bool PistacheHandlerInterface::getInteger(const std::string &name,
                                          int &target,
                                          bool zeroAllowed,
                                          bool ifMissingThenSendResponse)
{
    if (handlerType == TypeGet)
    {
        ExtPistache::getInt(*request, *response, name, target, ifMissingThenSendResponse);
    }
    if (handlerType == TypePost)
    {
        ExtRapidJSON postData(postedData);
        ExtPistache::getPostInt(postData, *response, name, target, ifMissingThenSendResponse);
    }
    if (target == 0 && !zeroAllowed)
    {
        answer(Pistache::Http::Code::Bad_Request, name + std::string(" must be != 0"));
        return false;
    }
    return true;
}

bool PistacheHandlerInterface::getString(const std::string &name,
                                         std::string &target,
                                         bool ifMissingThenSendResponse)
{
    return ExtPistache::getString(*request, *response, name, target, ifMissingThenSendResponse);
}

bool PistacheHandlerInterface::getPostString(const std::string &name,
                                             std::string &target,
                                             bool ifMissingThenSendResponse)
{
    ExtRapidJSON postData(postedData);
    return ExtPistache::getPostString(postData, *response, name, target, ifMissingThenSendResponse);
}

bool PistacheHandlerInterface::checkLogin()
{
    return true;
}

