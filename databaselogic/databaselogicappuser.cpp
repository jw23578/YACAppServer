#include "databaselogicappuser.h"
#include "postgres/pgexecutor.h"

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

