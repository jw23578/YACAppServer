#include "extpistache.h"
#include "utils/extstring.h"
#include "rapidjson/writer.h"

ExtPistache::ExtPistache()
{

}

void ExtPistache::answer(Pistache::Http::ResponseWriter &response,
                         Pistache::Http::Code code,
                         const std::string &message)
{
    std::map<std::string, std::string> empty;
    answer(response, code, message, empty);
}

void ExtPistache::answer(Pistache::Http::ResponseWriter &response,
                         Pistache::Http::Code code,
                         const std::string &message,
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
           d);
}

void ExtPistache::answer(Pistache::Http::ResponseWriter &response,
                         Pistache::Http::Code code,
                         rapidjson::Document &d)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    buffer.Put('\n');
    response.send(code, buffer.GetString(), MIME(Application, Json));
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
            answer(response, Pistache::Http::Code::Bad_Request, std::string("Missing ") + name);
        }
        return false;
    }
    target = ExtString::urlDecode(target);
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

    if (!postData.hasValue(name))
    {
        if (ifMissingThenSendResponse)
        {
            answer(response, Pistache::Http::Code::Bad_Request, std::string("Missing ") + name);
        }
        return false;
    }
    target = postData.getString(name);
    target = ExtString::urlDecode(target);
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
            answer(response, Pistache::Http::Code::Bad_Request, std::string("Missing ") + name);
        }
        return false;
    }
    target = postData.getInt(name);
    return true;
}