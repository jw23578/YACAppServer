#include "databaselogicappuser.h"
#include "postgres/pgexecutor.h"

void DatabaseLogicAppUser::loginSuccessful(const sole::uuid &appUserId,
                                           std::string &loginToken)
{
    if (loginToken.size() == 0)
    {
        loginToken = sole::uuid4().str();
    }
    int validHours(24 * 7);
    PGSqlString sql(utils.createInsertString(tableNames.t0009_appuser_logintoken));
    sql.set("id", sole::uuid4());
    sql.set("appuser_id", appUserId);
    sql.set("logintoken", loginToken);
    sql.set("login_token_valid_until", std::chrono::system_clock::now() + std::chrono::hours(1) * validHours);
    PGExecutor e(pool, sql);
}

bool DatabaseLogicAppUser::lookupAppUser(const std::string &loginEMail,
                                         sole::uuid &appUserId,
                                         std::string &message)
{
    auto it(loginEMail2AppUserId.find(loginEMail));
    if (it != loginEMail2AppUserId.end())
    {
        appUserId = it->second;
        return true;
    }
    std::map<std::string, sole::uuid> loginEMail2AppUserId;
    PGExecutor appUser(pool);
    if (!appUser.select(tableNames.t0003_appuser_profiles,
                        "loginemail",
                        loginEMail))
    {
        message = "LoginEMail/User not found";
        return false;
    }
    if (appUser.size() > 1)
    {
        message = std::string("more than one user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found. This is definitely a fatal error!");
        return false;
    }
    if (appUser.isNull("verified"))
    {
        message = "LoginEMail/User not yet verified";
        return false;
    }
    loginEMail2AppUserId[loginEMail] = appUser.uuid("id");
    return true;
}

DatabaseLogicAppUser::DatabaseLogicAppUser(LogStatController &logStatController,
                                           PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    utils(pool)
{

}

bool DatabaseLogicAppUser::appUserExists(const std::string &loginEMail)
{
    return utils.entryExists(tableNames.t0003_appuser_profiles, "loginemail", loginEMail);
}

bool DatabaseLogicAppUser::createAppUser(sole::uuid const &appId,
                                                const std::string &loginEMail,
                                                const std::string &password,
                                                std::string &message,
                                                std::string &verifyToken)
{
    if (!utils.entryExists(tableNames.t0002_apps, "id", appId.str()))
    {
        message = "App with id: " + appId.str() + " does not exist";
        return false;
    }
    sole::uuid appUserId(sole::uuid4());
    verifyToken = ExtString::randomString(0, 0, 4, 0);
    {
        PGSqlString sql(utils.createInsertString(tableNames.t0003_appuser_profiles));
        sql.set("id", appUserId);
        sql.set("app_id", appId);
        sql.set("loginemail", loginEMail);
        sql.set("verify_token", verifyToken);
        sql.set("verify_token_valid_until", std::chrono::system_clock::now() + std::chrono::minutes(60));
        PGExecutor e(pool, sql);
    }

    {
        PGSqlString sql("insert into t0004_appuser_passwordhashes "
                        " (id, appuser_id, password_hash) "
                        " values "
                        " (:id, :appuser_id, crypt(:password, gen_salt('bf')) ");
        sql.set("id", sole::uuid4());
        sql.set("appuser_id", appUserId);
        sql.set("password", password);
        PGExecutor e(pool, sql);
    }

    return true;
}

bool DatabaseLogicAppUser::verifyAppUser(const std::string &loginEMail,
                                         const std::string &verifyToken,
                                         std::string &message, std::string &loginToken)
{
    PGExecutor e(pool);
    e.select(tableNames.t0003_appuser_profiles,
             "loginemail",
             loginEMail);
    if (e.size() == 0)
    {
        message = std::string("no appuser with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found");
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
        PGExecutor e(pool);
        e.erase(tableNames.t0003_appuser_profiles,
                "loginemail",
                loginEMail);
        return false;
    }
    if (e.string("verify_token") != verifyToken)
    {
        message = std::string("wrong verifyToken");
        return false;
    }

    {
        PGSqlString sql("update ");
        sql += tableNames.t0003_appuser_profiles;
        sql += "set verified = now(), "
               "verify_token = '', "
               "verify_token_valid_until = null "
               "where loginemail = :loginemail ";
        sql.set("loginemail", loginEMail);
        PGExecutor e(pool, sql);
    }
    loginSuccessful(e.uuid("id"), loginToken);
    return true;
}

bool DatabaseLogicAppUser::loginAppUser(const std::string &loginEMail,
                                        const std::string &password,
                                        std::string &message,
                                        std::string &loginToken)
{
    sole::uuid appUserId;
    if (!lookupAppUser(loginEMail, appUserId, message))
    {
        return false;
    }
    PGExecutor login(pool);
    if (!login.login(tableNames.t0004_appuser_passwordhashes,
                     "password_hash",
                     password,
                     "appuser_id",
                     appUserId.str()))
    {
        message = "LoginEMail/User not found";
        return false;

    }
    if (!login.boolean("login_ok"))
    {
        message = "Password is wrong";
        return false;
    }

    loginSuccessful(appUserId, loginToken);
    return true;
}

bool DatabaseLogicAppUser::appUserLoggedIn(const std::string &loginEMail,
                                           const std::string &loginToken,
                                           sole::uuid &userId,
                                           std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::string ignored;
    if (!lookupAppUser(loginEMail, userId, ignored))
    {
        return false;
    }
    PGSqlString sql("select * from ");
    sql += tableNames.t0009_appuser_logintoken;
    sql += " where appuser_id = :appuser_id "
           "and login_token = :login_token";
    sql.set("appuser_id", userId);
    sql.set("login_token", loginToken);
    PGExecutor e(pool, sql);
    if (e.size() == 0)
    {
        return false;
    }
    userId = e.uuid("id");
    loginTokenValidUntil = e.timepoint("login_token_valid_until");
    return true;
}

void DatabaseLogicAppUser::refreshAppUserLoginToken(const std::string &loginEMail,
                                                    std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::string ignored;
    sole::uuid appUserId;
    if (!lookupAppUser(loginEMail, appUserId, ignored))
    {
        return;
    }

    PGSqlString sql("update ");
    sql += tableNames.t0009_appuser_logintoken;
    sql += " set login_token_valid_until = now() + interval '1 hour' *:validhours "
           "where appuser_id = :appuserid "
           "returning login_token_valid_until";
    MACRO_set(appUserId);
    int validHours(24 * 7);
    MACRO_set(validHours);
    PGExecutor e(pool, sql);
    loginTokenValidUntil = e.timepoint("login_token_valid_until");
}

