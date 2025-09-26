#include <iostream>
#define DOCTEST_CONFIG_IMPLEMENT
#include "thirds/doctest/doctest/doctest.h"

#include "yacappserver.h"
#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include "extrapidjson.h"
#include "pgconnectionpool.h"
#include "databaselogicuserandapp.h"
#include "emaillogic.h"
#include "logstatcontroller.h"
#include "filelogger.h"
#include "coutlogger.h"
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
#include "pgormsqlimplementation.h"
#include "JWUtils/extstringview.h"
#include "beginendtrack.h"
#include "coutlogger.h"
#include "postgres/pgormpersistence.h"
#include "orm_implementions/t0003_user_passwordhashes.h"
#include "tests/testviacurl.h"

ORMPersistenceInterface *testOPI = 0;
void testViaCurlMethod()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    TestViaCurl tvc(23578, "http://127.0.0.1", *testOPI);
    std::string rm;
    tvc.run(rm);
}

using namespace std;

int main(int argc, char **argv)
{
    LogStatController logStatController(LogStatController::verbose,
                                        "server",
                                        "yacapp");
    logStatController.add(new coutLogger);
    coutLogger::ActivateVisibleLogging a;

    bool runTestsViaCurl(false);
    bool runTests(false);
    bool cleanup(false);
    for (int i(0); i < argc; ++i)
    {
        std::string arg(argv[i]);
        if (arg == "runTestsViaCurl")
        {
            runTestsViaCurl = true;
        }
        if (arg == "runTests")
        {
            runTests = true;
        }
        if (arg == "cleanup")
        {
            cleanup = true;
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
    if (!json.hasObject("superUser"))
    {
        std::cerr << "missing superUser-Object in " << configFilename << std::endl;
        return 1;
    }
    ExtRapidJSON superUser(json.getObject("superUser"));
    if (superUser.getString("password").size() == 0)
    {
        std::cerr << "missing superUser-Password in " << configFilename << std::endl;
        return 1;
    }

    if (!ExtString::emailIsValid(superUser.getString("loginEMail")))
    {
        std::cerr << superUser.getString("loginEMail") << " is not a valid email-adress" << std::endl;
        return 1;
    }

    if (!json.hasObject("createrApp"))
    {
        std::cerr << "missing createrApp-Object in " << configFilename << std::endl;
        return 1;
    }
    ExtRapidJSON createrApp(json.getObject("createrApp"));
    if (createrApp.getString("app_id").size() == 0)
    {
        std::cerr << "missing app_id in " << configFilename << std::endl;
        return 1;
    }
    if (createrApp.getString("app_name").size() == 0)
    {
        std::cerr << "missing app_name in " << configFilename << std::endl;
        return 1;
    }

    std::string fileLoggerPath(json.getString("fileLoggerPath"));
    if (fileLoggerPath.size())
    {
        logStatController.add(new FileLogger(fileLoggerPath));
    }
    PGConnectionPool *superPool(0);
    if (json.hasObject("postgresSuperUser"))
    {
        ExtRapidJSON postgresSuperUser(json.getObject("postgresSuperUser"));
        superPool = new PGConnectionPool(postgresSuperUser.getString("host"),
                                         postgresSuperUser.getInt("port"),
                                         "postgres",
                                         postgresSuperUser.getString("user"),
                                         postgresSuperUser.getString("password"),
                                         1);

        PGORMSqlImplementation superSqlImplementation(*superPool);
        PGORMPersistence superOpi(superSqlImplementation);
        superOpi.createRole("test", "test");
        superOpi.createDatabase("test", "test");
        superOpi.dropDatabase("test");
        superOpi.dropRole("test");

        if (cleanup)
        {
            superOpi.dropDatabase("yacapp_database");
            superOpi.createDatabase("yacapp_database", "yacapp_user");
        }
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
    PGORMSqlImplementation sqlImplementation(pool);
    PGORMPersistence opi(sqlImplementation);
    testOPI = &opi;
    opi.initDatabase();
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

    // check for superuser
    t0002_user superUserProfile;
    if (!opi.selectObject({{superUserProfile.super_user.name(), "true"}}, superUserProfile))
    {
        superUserProfile.fstname = "superUser";
        superUserProfile.surname = "superUser";
        superUserProfile.loginemail = superUser.getString("loginEMail");
        superUserProfile.super_user = true;
        superUserProfile.verified = TimePointPostgreSqlNow;
        superUserProfile.app_id = NullUuid;
        superUserProfile.prepareFirstInsert();
        opi.insertObject(superUserProfile, superUserProfile.user_id);

        t0003_user_passwordhashes passwordHash;
        passwordHash.user_id = superUserProfile.user_id;
        passwordHash.password_hash = superUser.getString("password");
        passwordHash.app_id = NullUuid;
        passwordHash.prepareFirstInsert();
        opi.insertObject(passwordHash, superUserProfile.user_id);
    }
    t0001_apps theCreaterApp;
    std::string appId(createrApp.getString("app_id"));
    if (!opi.selectObject({{theCreaterApp.app_id.name(), appId}}, theCreaterApp))
    {
        theCreaterApp.app_id = appId;
        theCreaterApp.app_name = createrApp.getString("app_name");
        opi.insertObject(theCreaterApp, superUserProfile.user_id);
    }

    ORMVector<t0001_apps> allApps;
    opi.fetchAllObjects(allApps);
    for (size_t i(0); i < allApps.size(); ++i)
    {
        allApps[i].createDefaults(opi);
    }


    DatabaseLogicUserAndApp databaseLogicUserAndApp(logStatController,
                                                    pool,
                                                    opi);



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

    coutLogger::ActivateVisibleLogging a2;


    if (runTestsViaCurl)
    {
        std::thread *testThread(new std::thread(testViaCurlMethod));
    }

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
