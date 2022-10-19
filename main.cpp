#include <iostream>
#include "yacappserver.h"
#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include "utils/extrapidjson.h"
#include "postgres/pgconnectionpool.h"
#include "databaselogic.h"
#include "emaillogic.h"

using namespace std;

int main(int argc, char **argv)
{
    std::string configFilename("YACAppServerConfig.json");
    if (argc > 1)
    {
        configFilename = argv[1];
    }
    std::ifstream config(configFilename);
    if (!config)
    {
        std::cerr << configFilename << " not found\n";
        return 1;
    }
    rapidjson::IStreamWrapper configWrapper(config);
    rapidjson::Document configJSON;
    configJSON.ParseStream(configWrapper);
    if (configJSON.HasParseError())
    {
        std::cerr << configFilename << " does not contain valid JSON\n";
        return 1;
    }
    ExtRapidJSON json(configJSON);

    PGConnectionPool pool(json.getString("postgresHost"),
                          json.getInt("postgresPort"),
                          json.getString("postgresDBName"),
                          json.getString("postgresUser"),
                          json.getString("postgresPassword"),
                          10);
    DatabaseLogic databaseLogic(pool);
    EMailLogic emailLogic(json.getString("smtpSenderName"),
                          json.getString("smtpSenderEMail"),
                          json.getString("smtpReplyTo"),
                          json.getString("smtpHost"),
                          json.getString("smtpUser"),
                          json.getString("smtpPassword"));

    YACAppServer server(databaseLogic,
                        emailLogic,
                        json.getInt("serverPort"));
    return 0;
}
