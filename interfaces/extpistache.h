#ifndef EXTPISTACHE_H
#define EXTPISTACHE_H

#include "pistache/router.h"
#include "rapidjson/document.h"
#include "extrapidjson.h"

class ExtPistache
{
public:
    ExtPistache();
    bool prettyJson;

    void answerRaw(Pistache::Http::ResponseWriter &response,
                   Pistache::Http::Code code,
                   std::string const &data,
                   Pistache::Http::Mime::MediaType mediaType);

    void answer(Pistache::Http::ResponseWriter &response,
                Pistache::Http::Code code,
                std::string const &message,
                bool success,
                int missingRight);
    void answer(Pistache::Http::ResponseWriter &response,
                Pistache::Http::Code code,
                std::string const &message,
                bool success,
                int missingRight,
                std::map<std::string, std::string> &data);
    void answer(Pistache::Http::ResponseWriter &response,
                Pistache::Http::Code code,
                bool success,
                int missingRight,
                rapidjson::Document &d);

    bool getString(const Pistache::Rest::Request &request,
                   Pistache::Http::ResponseWriter &response,
                   std::string const &name,
                   std::string &target,
                   bool ifMissingThenSendResponse);
    bool getBool(const Pistache::Rest::Request &request,
                 Pistache::Http::ResponseWriter &response,
                 std::string const &name,
                 bool &target,
                 bool ifMissingThenSendResponse);
    bool getInt(const Pistache::Rest::Request &request,
                Pistache::Http::ResponseWriter &response,
                std::string const &name,
                int &target,
                bool ifMissingThenSendResponse);

    bool getPostString(ExtRapidJSON &postData,
                       Pistache::Http::ResponseWriter &response,
                       std::string const &name,
                       std::string &target,
                       bool ifMissingThenSendResponse);
    bool getPostInt(ExtRapidJSON &postData,
                    Pistache::Http::ResponseWriter &response,
                    std::string const &name,
                    int &target,
                    bool ifMissingThenSendResponse);
    bool getPostBool(ExtRapidJSON &postData,
                     Pistache::Http::ResponseWriter &response,
                     std::string const &name,
                     bool &target,
                     bool ifMissingThenSendResponse);
};

#endif // EXTPISTACHE_H
