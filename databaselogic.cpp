#include "databaselogic.h"
#include "postgres/pgsqlstring.h"
#include "postgres/pgcommandtransactor.h"
#include "utils/extstring.h"

DatabaseLogic::DatabaseLogic(PGConnectionPool &pool):pool(pool)
{

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
                    " (id, loginemail, password_hash, verified, verify_token, login_token, login_token_valid_until) "
                    " values "
                    " (:id, :loginEMail, crypt(:password, gen_salt('bf')),"
                    " null,"
                    " :verify_token,"
                    " '',"
                    " null) ");
    sql.set("id", sole::uuid4());
    sql.set("loginEMail", loginEMail);
    sql.set("password", password);
    std::string verify_token(ExtString::randomString(0, 0, 4, 0));
    sql.set("verify_token", verify_token);
    pqxx::result r;
    PGCommandTransactor ct(pool, sql, r);
    return verify_token;
}
