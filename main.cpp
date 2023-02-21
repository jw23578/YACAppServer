#include <iostream>
#define DOCTEST_CONFIG_IMPLEMENT
#include "thirds/doctest/doctest/doctest.h"

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
#include "databaselogics.h"
#include "tests/testdatabaselogicmessages.h"
#include "tests/testdatabaselogicappuser.h"
#include "tests/testdatabaselogicworktime.h"
#include "tests/testdatabaselogicimagetable.h"

#include "curlWrapper/jw78firebasewrapper.h"
#include "rapidjson/writer.h"

using namespace std;

int main(int argc, char **argv)
{
    bool runTests(false);
    for (int i(0); i < argc; ++i)
    {
        if (std::string(argv[i]) == "runTests")
        {
            runTests = true;
        }
    }
    std::srand(std::time(nullptr));
    if (runTests)
    {
        doctest::Context context;
        context.applyCommandLine(argc, argv);
        context.setOption("no-breaks", true);
        int res = context.run();

        if (res != 0)
        {
            return res;
        }
    }

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
    DatabaseLogics databaseLogics(logStatController,
                                  pool);

    DatabaseLogicMessages databaseLogicMessages(logStatController,
                                                pool);

    if (runTests)
    {
        TestDatabaseLogicMessages testDatabaseLogicMessage(databaseLogicMessages);
        TestDatabaseLogicAppUser testDatabaseLogicAppUser(databaseLogics.databaseLogicAppUser);
        TestDatabaseLogicImageTable testDatabaseLogicImageTable(databaseLogics.databaseLogicImageTable);
        if (!testDatabaseLogicAppUser)
        {
            return 1;
        }
    }
//    TestDatabaseLogicWorktime testDatabaseLogicWorktime(logStatController, databaseLogics);


    EMailLogic emailLogic(json.getString("smtpSenderName"),
                          json.getString("smtpSenderEMail"),
                          json.getString("smtpReplyTo"),
                          json.getString("smtpHost"),
                          json.getString("smtpUser"),
                          json.getString("smtpPassword"));

    YACAppServer server(json.getString("firebaseApiKey"),
                        databaseLogics,
                        databaseLogicTables,
                        databaseLogicUserAndApp,
                        databaseLogics.databaseLogicAppUser,
                        databaseLogicMessages,
                        emailLogic,
                        json.getInt("serverPort"));
    return 0;
}

TEST_CASE("test")
{

}
