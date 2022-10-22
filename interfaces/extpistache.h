#ifndef EXTPISTACHE_H
#define EXTPISTACHE_H

#include "pistache/router.h"
#include "rapidjson/document.h"
#include "utils/extrapidjson.h"

class ExtPistache
{
public:
    ExtPistache();

    static void answer(Pistache::Http::ResponseWriter &response,
                       Pistache::Http::Code code,
                       std::string const &message);
    static void answer(Pistache::Http::ResponseWriter &response,
                       Pistache::Http::Code code,
                       std::string const &message,
                       std::map<std::string, std::string> &data);
    static void answer(Pistache::Http::ResponseWriter &response,
                       Pistache::Http::Code code,
                       rapidjson::Document &d);

    static bool getString(const Pistache::Rest::Request &request,
                          Pistache::Http::ResponseWriter &response,
                          std::string const &name,
                          std::string &target,
                          bool ifMissingThenSendResponse);
    static bool getBool(const Pistache::Rest::Request &request,
                        Pistache::Http::ResponseWriter &response,
                        std::string const &name,
                        bool &target,
                        bool ifMissingThenSendResponse);
    static bool getInt(const Pistache::Rest::Request &request,
                       Pistache::Http::ResponseWriter &response,
                       std::string const &name,
                       int &target,
                       bool ifMissingThenSendResponse);

    static bool getPostString(ExtRapidJSON &postData,
                              Pistache::Http::ResponseWriter &response,
                              std::string const &name,
                              std::string &target,
                              bool ifMissingThenSendResponse);
    static bool getPostInt(ExtRapidJSON &postData,
                           Pistache::Http::ResponseWriter &response,
                           std::string const &name,
                           int &target,
                           bool ifMissingThenSendResponse);
};

#endif // EXTPISTACHE_H
