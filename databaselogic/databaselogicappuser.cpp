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

std::string DatabaseLogicAppUser::createAppUser(const std::string &loginEMail,
                                                const std::string &password)
{
    sole::uuid appUserId(sole::uuid4());
    std::string verify_token(ExtString::randomString(0, 0, 4, 0));
    {
        PGSqlString sql(utils.createInsertString(tableNames.t0003_appuser_profiles));
        sql.set("id", appUserId);
        sql.set("loginemail", loginEMail);
        sql.set("verify_token", verify_token);
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

    return verify_token;
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

