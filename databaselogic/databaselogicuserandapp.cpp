#include "databaselogicuserandapp.h"
#include "postgres/pgsqlstring.h"
#include "postgres/pgexecutor.h"
#include "utils/extstring.h"
#include "postgres/pgutils.h"
#include "pgoidstorer.h"
#include "pgoidloader.h"

void DatabaseLogicUserAndApp::loginSuccessful(const std::string &loginEMail,
                                    std::string &loginToken)
{
    if (loginToken.size() == 0)
    {
        loginToken = sole::uuid4().str();
    }
    int validHours(24 * 7);
    PGSqlString sql("update t0001_users "
                    "set login_token = :login_token, "
                    "login_token_valid_until = now() + interval '1 hour' *:validHours "
                    "where loginemail = :loginemail");
    sql.set("loginemail", loginEMail);
    sql.set("validHours", validHours);
    sql.set("login_token", loginToken);
    PGExecutor e(pool, sql);
}

DatabaseLogicUserAndApp::DatabaseLogicUserAndApp(LogStatController &logStatController,
                             PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    utils(pool)
{

}

bool DatabaseLogicUserAndApp::userExists(const std::string &loginEMail)
{
    return utils.entryExists(tableNames.t0001_users, "loginemail", loginEMail);
}

std::string DatabaseLogicUserAndApp::createUser(const std::string &loginEMail,
                                      const std::string &password)
{
    PGSqlString sql("insert into t0001_users "
                    " (id, loginemail, password_hash, verified, verify_token, verify_token_valid_until, login_token, login_token_valid_until) "
                    " values "
                    " (:id, :loginEMail, crypt(:password, gen_salt('bf')),"
                    " null,"
                    " :verify_token,"
                    " :verify_token_valid_until, "
                    " '',"
                    " null) ");
    sql.set("id", sole::uuid4());
    sql.set("loginEMail", loginEMail);
    sql.set("password", password);
    std::string verify_token(ExtString::randomString(0, 0, 4, 0));
    sql.set("verify_token", verify_token);
    sql.set("verify_token_valid_until", std::chrono::system_clock::now() + std::chrono::minutes(60));
    PGExecutor e(pool, sql);
    return verify_token;
}

bool DatabaseLogicUserAndApp::verifyUser(const std::string &loginEMail,
                               const std::string &verifyToken,
                               std::string &message,
                               std::string &loginToken)
{
    PGSqlString sql("select * from t0001_users "
                    "where loginemail = :loginemail ");
    sql.set("loginemail", loginEMail);
    PGExecutor e(pool, sql);
    if (e.size() == 0)
    {
        message = std::string("no user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found");
        return false;
    }
    if (e.size() > 1)
    {
        message = std::string("more than one user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found. This is definitely a fatal error!");
        return false;
    }
    logStatController.log(__FILE__, __LINE__, LogStatController::verbose,
                          std::string("verify_token_valid_until as string: ") + e.string("verify_token_valid_until"));
    std::chrono::system_clock::time_point verify_token_valid_until(e.timepoint("verify_token_valid_until"));
    {
        std::time_t t = std::chrono::system_clock::to_time_t(verify_token_valid_until);
        std::stringstream ss;
        ss << std::put_time( std::localtime( &t ), "%FT%T%z" );
        logStatController.log(__FILE__, __LINE__, LogStatController::verbose,
                              std::string("verify_token_valid_until: ") + ss.str());
    }
    std::chrono::system_clock::time_point now(std::chrono::system_clock::now());
    {
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time( std::localtime( &t ), "%FT%T%z" );
        logStatController.log(__FILE__, __LINE__, LogStatController::verbose,
                              std::string("now: ") + ss.str());
    }
    if (verify_token_valid_until < now)
    {
        message = std::string("verify token not valid any more, please register again");
        PGSqlString delSql("delete from t0001_users "
                           "where loginemail = :loginemail ");
        delSql.set("loginemail", loginEMail);
        PGExecutor e(pool, delSql);
        return false;
    }
    if (e.string("verify_token") != verifyToken)
    {
        message = std::string("wrong verifyToken");
        return false;
    }

    sql = "update t0001_users "
          "set verified = now(), "
          "verify_token = '', "
          "verify_token_valid_until = null "
          "where loginemail = :loginemail ";
    sql.set("loginemail", loginEMail);
    e.exec(sql);
    loginSuccessful(loginEMail, loginToken);
    return true;
}

bool DatabaseLogicUserAndApp::loginUser(const std::string &loginEMail,
                              const std::string &password,
                              std::string &message,
                              std::string &loginToken)
{
    PGSqlString sql("select *, password_hash = crypt(:password, password_hash) as login_ok "
                    "from t0001_users "
                    "where loginemail = :loginemail ");
    sql.set("loginemail", loginEMail);
    sql.set("password", password);
    PGExecutor e(pool, sql);
    if (!e.size())
    {
        message = "LoginEMail/User not found. Please check your LoginEMail or register first.";
        return false;
    }
    if (e.isNull("verified"))
    {
        message = "LoginEMail/User not yet verified";
        return false;
    }
    if (!e.boolean("login_ok"))
    {
        message = "Password is wrong";
        return false;
    }
    loginToken = e.string("login_token");
    loginSuccessful(loginEMail, loginToken);
    return true;
}

bool DatabaseLogicUserAndApp::userLoggedIn(const std::string &loginEMail,
                                 const std::string &loginToken,
                                 sole::uuid &userId,
                                 std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    PGSqlString sql("select * from t0001_users "
                    "where loginemail = :loginemail "
                    "and login_token = :login_token");
    sql.set("login_token", loginToken);
    sql.set("loginemail", loginEMail);
    PGExecutor e(pool, sql);
    if (e.size() == 0)
    {
        return false;
    }
    userId = e.uuid("id");
    loginTokenValidUntil = e.timepoint("login_token_valid_until");
    return true;
}

void DatabaseLogicUserAndApp::refreshLoginToken(const std::string &loginEMail,
                                      std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    PGSqlString sql("update t0001_users "
                    "set login_token_valid_until = now() + interval '1 hour' *:validhours "
                    "where loginemail = :loginemail "
                    "returning login_token_valid_until");
    MACRO_set(sql, loginEMail);
    sql.set("loginEMail", loginEMail);
    int validHours(24 * 7);
    MACRO_set(sql, validHours);
    PGExecutor e(pool, sql);
    loginTokenValidUntil = e.timepoint("login_token_valid_until");
}

bool DatabaseLogicUserAndApp::saveApp(const sole::uuid owner_id,
                            const std::string &app_id,
                            const std::string &app_name,
                            const int app_version,
                            const std::string &app_logo_url,
                            const std::string &app_color_name,
                            const bool is_template_app,
                            const std::string &json_yacapp,
                            const std::basic_string<std::byte> &yacpck_base64,
                            std::string &message)
{
    std::string app_id_field("id");
    PGUtils utils(pool);
    PGSqlString sql(utils.createEntryExistsString(tableNames.t0002_apps, app_id_field));
    sql.set("id", app_id);
    PGExecutor e(pool, sql);
    if (e.size())
    {
        if (e.uuid("owner_id") != owner_id)
        {
            message = "user is not app owner";
            return false;
        }
        sql = utils.createUpdateString(tableNames.t0002_apps, app_id_field);
    }
    else
    {
        sql = utils.createInsertString(tableNames.t0002_apps);
    }
    pqxx::oid oid;
    PGOidStorer storeOid(pool, yacpck_base64, oid);
    sql.set("yacpck_base64", oid);
    sql.set("owner_id", owner_id);
    sql.set("id", app_id);
    MACRO_set(sql, app_name);
    MACRO_set(sql, app_version);
    MACRO_set(sql, app_logo_url);
    MACRO_set(sql, app_color_name);
    MACRO_set(sql, is_template_app);
    MACRO_set(sql, json_yacapp);
    PGExecutor insertOrUpdate(pool, sql);
    return true;
}

size_t DatabaseLogicUserAndApp::fetchAllAPPs(rapidjson::Document &target)
{
    auto &alloc(target.GetAllocator());
    PGSqlString sql("select id "
                    ", app_name "
                    ", app_version "
                    ", app_logo_url "
                    ", app_color_name "
                    "from t0002_apps "
                    "order by app_name ");
    PGExecutor e(pool, sql);
    rapidjson::Value allAPPs(rapidjson::kArrayType);
    for (size_t r(0); r < e.size(); ++r)
    {
        rapidjson::Value appObject;
        appObject.SetObject();
        appObject.AddMember("app_id", e.string("id"), alloc);
        appObject.AddMember("app_name", e.string("app_name"), alloc);
        appObject.AddMember("app_version", e.integer("app_version"), alloc);
        appObject.AddMember("app_logo_url", e.string("app_logo_url"), alloc);
        appObject.AddMember("app_color_name", e.string("app_color_name"), alloc);
        allAPPs.PushBack(appObject, alloc);
        e.next();
    }
    target.SetObject();
    target.AddMember("allApps", allAPPs, alloc);
    return e.size();
}

bool DatabaseLogicUserAndApp::fetchOneApp(const std::string &app_id,
                                const int current_installed_version,
                                rapidjson::Document &target)
{
    target.SetObject();

    auto &alloc(target.GetAllocator());
    PGSqlString sql("select app_name "
                    ", app_version "
                    ", json_yacapp "
                    ", yacpck_base64 "
                    "from t0002_apps "
                    "where id = :app_id ");
    MACRO_set(sql, app_id);
    PGExecutor e(pool, sql);
    if (!e.size())
    {
        target.AddMember("message", "app not found", alloc);
        return false;
    }
    if (e.integer("app_version") <= current_installed_version)
    {
        target.AddMember("message", "app version is up to date", alloc);
        return true;
    }
    target.AddMember("app_id", app_id, alloc);
    target.AddMember("app_name", e.string("app_name"), alloc);
    target.AddMember("message", "app found", alloc);
    target.AddMember("json_yacapp", e.string("json_yacapp"), alloc);
    std::string yacpck_base64;
    PGOidLoader loader(pool,
                       e.oid("yacpck_base64"),
                       yacpck_base64);
    target.AddMember("yacpck_base64", yacpck_base64, alloc);
    return true;
}
