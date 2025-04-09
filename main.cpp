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
#include "logstat/coutlogger.h"
#include "databaselogictables.h"
#include "databaselogicappuser.h"
#include "databaselogicmessages.h"
#include "databaselogics.h"
#include "tests/testdatabaselogics.h"
#include "tests/testdatabaselogicmessages.h"
#include "tests/testdatabaselogicappuser.h"
#include "tests/testdatabaselogicworktime.h"
#include "tests/testdatabaselogicimagetable.h"
#include "tests/testorm2postgres.h"

#include "curlWrapper/jw78firebasewrapper.h"
#include "rapidjson/writer.h"
#include "orm_implementions/yacormfactory.h"
#include "pgsqlimplementation.h"
#include "JWUtils/extstringview.h"
#include "logstat/beginendtrack.h"
#include "logstat/coutlogger.h"


using namespace std;

int main(int argc, char **argv)
{
    LogStatController logStatController(LogStatController::verbose,
                                        "server",
                                        "yacapp");
    logStatController.add(new coutLogger);
    coutLogger::ActivateVisibleLogging a;

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

    std::string configFilename(ExtString::extractFilePath(argv[0]) + "/YACAppServerConfig.json");
    LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, MACRO_NV(configFilename));
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
                          10);
    if (!json.getBool("visibleLogActive"))
    {
        a.stop();
    }


    YACORMFactory factory;
    PGSqlImplementation sqlImplementation(pool);
    ORMPersistenceInterface opi(sqlImplementation);
    DatabaseLogicTables databaseLogicTables(logStatController,
                                            pool,
                                            factory);
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

    ORMVector<t0002_apps> allApps;
    opi.fetchAllObjects(allApps);
    for (size_t i(0); i < allApps.size(); ++i)
    {
        allApps[i].createDefaults(opi);
    }


    DatabaseLogicUserAndApp databaseLogicUserAndApp(logStatController,
                                                    pool);



    DatabaseLogics databaseLogics(logStatController,
                                  pool,
                                  opi);


    DatabaseLogicMessages databaseLogicMessages(logStatController,
                                                pool);


    if (runTests)
    {
        coutLogger::ActivateVisibleLogging a;
        TestORM2Postgres testORM2Postgres(pool);
        a.stop();
        TestDatabaseLogics testDatabaseLogics(databaseLogics);
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
                        factory,
                        opi,
                        databaseLogics,
                        databaseLogicUserAndApp,
                        databaseLogics.databaseLogicAppUser,
                        emailLogic,
                        json.getInt("serverPort"),
                        json.getString("serverCertFilename"),
                        json.getString("serverKeyFilename"));
    return 0;
}

TEST_CASE("test")
{

}
