#include "databaselogic.h"
#include "postgres/pgconnection.h"
#include "postgres/pgsqlstring.h"
#include "postgres/pgexecutor.h"
#include "utils/extstring.h"
#include "postgres/pgutils.h"

void DatabaseLogic::loginSuccessful(const std::string &loginEMail,
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

DatabaseLogic::DatabaseLogic(PGConnectionPool &pool):pool(pool)
{

}

bool DatabaseLogic::connectionOk()
{
    try
    {
        PGConnection connection(pool);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool DatabaseLogic::pgCryptoInstalled()
{
    PGUtils utils(pool);
    return utils.pgCryptoInstalled();
}

void DatabaseLogic::createDatabaseTables()
{
    PGUtils utils(pool);
    if (!utils.tableExists(t0001_users))
    {
        PGSqlString sql("create table ");
        sql += t0001_users;
        sql += std::string(" ( id uuid, "
                           " loginemail text, "
                           " password_hash text, "
                           " verified timestamp, "
                           " verify_token text, "
                           " verify_token_valid_until timestamp, "
                           " login_token text, "
                           " login_token_valid_until timestamp,"
                           " primary key (id)) ");
        PGExecutor e(pool, sql);
    }
    std::string t0001_user_i1("t0001_user_i1");
    utils.createIndex(t0001_users, t0001_user_i1, "(loginemail)");

    if (!utils.tableExists(t0002_apps))
    {
        PGSqlString sql("create table ");
        sql += t0002_apps;
        sql += std::string("( id uuid, "
                           "appId uuid, "
                           "ownerId uuid, "
                           "json_yacapp text, "
                           "yacpck_base64 text, "
                           "primary key (id))");
        PGExecutor e(pool, sql);
    }
    std::string t0002_apps_i1;
    utils.createIndex(t0002_apps, t0002_apps_i1, "(appId)");
}


bool DatabaseLogic::userExists(const std::string &loginEMail)
{
    PGSqlString sql("select * from t0001_users "
                    "where loginemail = :loginemail");
    sql.set("loginemail", loginEMail);
    PGExecutor e(pool, sql);
    return e.size() > 0;
}

std::string DatabaseLogic::createUser(const std::string &loginEMail,
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

bool DatabaseLogic::verfiyUser(const std::string &loginEMail,
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
    if (e.timepoint("verify_token_valid_until") < std::chrono::system_clock::now())
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

bool DatabaseLogic::loginUser(const std::string &loginEMail,
                              const std::string &password,
                              std::string &message,
                              std::string &loginToken)
{
    PGSqlString sql("select * from t0001_users "
                    "where loginemail = :loginemail "
                    "and password_hash = crypt(:password, password_hash) ");
    sql.set("loginemail", loginEMail);
    sql.set("password", password);
    PGExecutor e(pool, sql);
    if (!e.size())
    {
        message = "password or loginemail wrong";
        return false;
    }
    if (e.isNull("verified"))
    {
        message = "user not yet verified";
        return false;
    }
    loginToken = e.string("login_token");
    loginSuccessful(loginEMail, loginToken);
    return true;
}

bool DatabaseLogic::userLoggedIn(const std::string &loginEMail,
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

void DatabaseLogic::refreshLoginToken(const std::string &loginEMail,
                                      std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    PGSqlString sql("update t0001_users "
                    "set login_token_valid_until = now() + interval '1 hour' *:validHours "
                    "where loginemail = :loginemail "
                    "returning login_token_valid_until");
    sql.set("loginEMail", loginEMail);
    PGExecutor e(pool, sql);
    loginTokenValidUntil = e.timepoint("login_token_valid_until");
}

bool DatabaseLogic::saveApp(const sole::uuid userId,
                            const std::string &appId,
                            const std::string &json_yacapp,
                            const std::string &yacpck_base64,
                            std::string &message)
{
    std::string appIdField("appId");
    PGUtils utils(pool);
    PGSqlString sql(utils.createEntryExistsString(t0002_apps, appIdField));
    MACRO_set(appId);
    PGExecutor e(pool, sql);
    if (e.size())
    {
        if (e.uuid("ownerid") != userId)
        {
            message = "user is not app owner";
            return false;
        }
        sql = utils.createUpdateString(t0002_apps, appIdField);
        sql.set("id", e.uuid("id"));
    }
    else
    {
        sql = utils.createInsertString(t0002_apps);
        sql.set("id", sole::uuid4());
    }
    sql.set("ownerid", userId);
    MACRO_set(appId);
    MACRO_set(json_yacapp);
    MACRO_set(yacpck_base64);
    PGExecutor insertOrUpdate(pool, sql);
    return true;
}
