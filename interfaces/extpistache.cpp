#include "extpistache.h"
#include "utils/extstring.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "logstat/logstatcontroller.h"

ExtPistache::ExtPistache()
{

}

void ExtPistache::answer(Pistache::Http::ResponseWriter &response,
                         Pistache::Http::Code code,
                         const std::string &message,
                         bool success,
                         int missingRight)
{
    std::map<std::string, std::string> empty;
    answer(response, code, message, success, missingRight, empty);
}

void ExtPistache::answer(Pistache::Http::ResponseWriter &response,
                         Pistache::Http::Code code,
                         const std::string &message,
                         bool success,
                         int missingRight,
                         std::map<std::string, std::string> &data)
{
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("message", rapidjson::StringRef(message.c_str()), d.GetAllocator());
    for (auto &m : data)
    {
        rapidjson::Value key(rapidjson::StringRef(m.first.c_str()));
        d.AddMember(key, rapidjson::StringRef(m.second.c_str()), d.GetAllocator());
    }
    answer(response,
           code,
           success,
           missingRight,
           d);
}

void ExtPistache::answer(Pistache::Http::ResponseWriter &response,
                         Pistache::Http::Code code,
                         bool success,
                         int missingRight,
                         rapidjson::Document &d)
{
    d.AddMember("missingRight", missingRight, d.GetAllocator());
    d.AddMember("success", success, d.GetAllocator());
    rapidjson::StringBuffer buffer;
    if (prettyJson)
    {
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        buffer.Put('\n');
    }
    else
    {
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        buffer.Put('\n');
    }
    const std::string responseString(buffer.GetString());
    LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("responseCode: ") + ExtString::toString((int)code) + " " + ExtString::toString(code));
    LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("responseStringSize: ") + ExtString::toString(responseString.size()));
    if (responseString.size() < 100)
    {
        LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("responseString: ") + responseString);
    }
    answerRaw(response, code, responseString, MIME(Application, Json));
}

void ExtPistache::answerRaw(Pistache::Http::ResponseWriter &response,
                            Pistache::Http::Code code,
                            std::string const &data,
                            Pistache::Http::Mime::MediaType mediaType)
{
    response.send(code, data, mediaType);
}



bool ExtPistache::getString(const Pistache::Rest::Request &request,
                            Pistache::Http::ResponseWriter &response,
                            const std::string &name,
                            std::string &target,
                            bool ifMissingThenSendResponse)
{
    auto query(request.query());
    auto theString(query.get(name));
    if (theString.has_value())
    {
        target = theString.value();
    }
    if (!target.size())
    {
        if (ifMissingThenSendResponse)
        {
            const int missingRight(0);
            answer(response, Pistache::Http::Code::Bad_Request, std::string("Missing ") + name, false, missingRight);
        }
        return false;
    }
    target = ExtString::urlDecode(target);
    ExtString::trim(target);
    return true;
}

bool ExtPistache::getBool(const Pistache::Rest::Request &request,
                          Pistache::Http::ResponseWriter &response,
                          const std::string &name,
                          bool &target,
                          bool ifMissingThenSendResponse)
{
    std::string s;
    if (!getString(request, response, name, s, ifMissingThenSendResponse))
    {
        return false;
    }
    s = ExtString::lower(s);
    target = s == "1" || s == "true";
    return true;
}

bool ExtPistache::getInt(const Pistache::Rest::Request &request,
                         Pistache::Http::ResponseWriter &response,
                         const std::string &name,
                         int &target,
                         bool ifMissingThenSendResponse)
{
    std::string s;
    if (!getString(request, response, name, s, ifMissingThenSendResponse))
    {
        return false;
    }
    ExtString::to(s, target);
    return true;
}

bool ExtPistache::getPostString(ExtRapidJSON &postData,
                                Pistache::Http::ResponseWriter &response,
                                const std::string &name,
                                std::string &target,
                                bool ifMissingThenSendResponse)
{
    if (postData.hasValue(name))
    {
        target = postData.getString(name);
    }
    if (!target.size())
    {
        if (ifMissingThenSendResponse)
        {
            const int missingRight(0);
            answer(response, Pistache::Http::Code::Bad_Request, std::string("Missing ") + name, false, missingRight);
        }
        return false;
    }
    target = ExtString::urlDecode(target);
    ExtString::trim(target);
    return true;
}

bool ExtPistache::getPostInt(ExtRapidJSON &postData,
                             Pistache::Http::ResponseWriter &response,
                             const std::string &name,
                             int &target,
                             bool ifMissingThenSendResponse)
{
    if (!postData.hasValue(name))
    {
        if (ifMissingThenSendResponse)
        {
            const int missingRight(0);
            answer(response, Pistache::Http::Code::Bad_Request, std::string("Missing ") + name, false, missingRight);
        }
        return false;
    }
    target = postData.getInt(name);
    return true;
}

bool ExtPistache::getPostBool(ExtRapidJSON &postData,
                              Pistache::Http::ResponseWriter &response,
                              const std::string &name,
                              bool &target,
                              bool ifMissingThenSendResponse)
{
    if (!postData.hasValue(name))
    {
        if (ifMissingThenSendResponse)
        {
            const int missingRight(0);
            answer(response, Pistache::Http::Code::Bad_Request, std::string("Missing ") + name, false, missingRight);
        }
        return false;
    }
    target = postData.getBool(name);
    return true;
}
