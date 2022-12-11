#include <iostream>
#include "yacappserver.h"
#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include "utils/extrapidjson.h"
#include "postgres/pgconnectionpool.h"
#include "databaselogicuserandapp.h"
#include "emaillogic.h"
#include "logstat/logstatcontroller.h"
#include "logstat/filelogger.h"
#include "databaselogictables.h"
#include "databaselogicappuser.h"
#include "databaselogicmessages.h"
#include "tests/testdatabaselogicmessages.h"
#include "tests/testtimepoint.h"
#include "tests/testdatabaselogicappuser.h"

using namespace std;

int main(int argc, char **argv)
{
    std::srand(std::time(nullptr));
    TestTimepoint ttp;

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

    LogStatController logStatController(LogStatController::verbose,
                                        "server",
                                        "yacapp");
    std::string fileLoggerPath(json.getString("fileLoggerPath"));
    if (fileLoggerPath.size())
    {
        logStatController.add(new FileLogger(fileLoggerPath));
    }

    PGConnectionPool pool(json.getString("postgresHost"),
                          json.getInt("postgresPort"),
                          json.getString("postgresDBName"),
                          json.getString("postgresUser"),
                          json.getString("postgresPassword"),
                          10,
                          logStatController);

    DatabaseLogicTables databaseLogicTables(logStatController,
                                            pool);

    std::cout << "Checking Databaseconnection\n";
    if (!databaseLogicTables.connectionOk())
    {
        std::cout << "Databaseconnection is not ok\n";
        std::cout << "exiting\n";
        return 1;
    }
    std::cout << "Databaseconnection is ok\n";
    std::cout << "Checking for PGCrypto installed\n";
    if (!databaseLogicTables.pgCryptoInstalled())
    {
        std::cout << "PGCrypto is not installed\n";
        std::cout << "exiting\n";
        return 1;
    }
    std::cout << "PGCrypto is installed\n";
    databaseLogicTables.createDatabaseTables();


    DatabaseLogicUserAndApp databaseLogicUserAndApp(logStatController,
                                                    pool);
    DatabaseLogicAppUser databaseLogicAppUser(logStatController,
                                              pool);

    DatabaseLogicMessages databaseLogicMessages(logStatController,
                                                pool);
//    TestDatabaseLogicMessages testDatabaseLogicMessage(databaseLogicMessages);
//    TestDatabaseLogicAppUser testDatabaseLogicAppUser(databaseLogicAppUser);


    EMailLogic emailLogic(json.getString("smtpSenderName"),
                          json.getString("smtpSenderEMail"),
                          json.getString("smtpReplyTo"),
                          json.getString("smtpHost"),
                          json.getString("smtpUser"),
                          json.getString("smtpPassword"));

    YACAppServer server(databaseLogicTables,
                        databaseLogicUserAndApp,
                        databaseLogicAppUser,
                        databaseLogicMessages,
                        emailLogic,
                        json.getInt("serverPort"));
    return 0;
}
