#include "databaselogic.h"
#include "postgres/pgsqlstring.h"
#include "postgres/pgcommandtransactor.h"
#include "utils/extstring.h"
#include "postgres/pgutils.h"

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
    std::string t0001_users("t0001_users");
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
                           " login_token_valid_until timestamp) ");
        pqxx::result r;
        PGCommandTransactor ct(pool, sql, r);
    }
}


bool DatabaseLogic::userExists(const std::string &loginEMail)
{
    PGSqlString sql("select * from t0001_users "
                    "where loginemail = :loginemail");
    sql.set("loginemail", loginEMail);
    pqxx::result r;
    PGCommandTransactor ct(pool, sql, r);
    return r.size() > 0;
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
    pqxx::result r;
    PGCommandTransactor ct(pool, sql, r);
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
    pqxx::result r;
    PGCommandTransactor ct(pool, sql, r);
    if (r.size() == 0)
    {
        message = std::string("no user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found");
        return false;
    }
    if (r.size() > 1)
    {
        message = std::string("more than one user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found. This is definitely a fatal error!");
        return false;
    }
    const pqxx::row &row(r[0]);
    if (row["verify_token"].get<std::string>() != verifyToken)
    {
        message = std::string("wrong verifyToken");
        return false;
    }
    std::chrono::system_clock::time_point verify_token_valid_until(ExtString::toTimepoint(row["verify_token_valid_until"].c_str()));
    if (verify_token_valid_until < std::chrono::system_clock::now())
    {
        message = std::string("verify token not valid any more");
        return false;
    }
}
