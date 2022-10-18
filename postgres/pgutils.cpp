#include "pgutils.h"
#include "pgcommandtransactor.h"
#include "pgsqlstring.h"
#include "utils/extstring.h"

PGUtils::PGUtils(PGConnectionPool &pool):pool(pool)
{

}

void PGUtils::alterTableAddColumn(const std::string &tableName,
                                  const std::string &columnName,
                                  const std::string &columnType) const
{
    PGSqlString sql("alter table ");
    sql += tableName;
    sql += " add column " + columnName + " " + columnType;
    pqxx::result result;
    PGCommandTransactor ct(pool, sql, result);
}

bool PGUtils::tableExists(const std::string &tableName) const
{
    PGSqlString sql("select table_name from information_schema.tables ");
    sql += "where table_schema = 'public' ";
    sql += "and table_name = :tableName";
    sql.set("tableName", ExtString::lower(tableName));
    pqxx::result result;
    PGCommandTransactor ct(pool,
                           sql,
                           result);
    return result.size() > 0;
}

bool PGUtils::tableEmpty(const std::string &tableName) const
{
    PGSqlString sql("select * from ");
    sql += tableName;
    pqxx::result result;
    PGCommandTransactor ct(pool,
                           sql,
                           result);
    return result.size() == 0;
}

bool PGUtils::databaseExists(const std::string &databaseName) const
{
    PGSqlString sql("select * from pg_database where datname = :datname");
    sql.set("datname", ExtString::lower(databaseName));
    pqxx::result result;
    PGCommandTransactor ct(pool,
                           sql,
                           result);
    return result.size() > 0;
}

bool PGUtils::createDatabase(const std::string &databaseName,
                             const std::string &owner) const
{
    PGSqlString sql("create database ");
    sql += databaseName + " owner " + owner;
    pqxx::result result;
    PGCommandTransactor ct(pool,
                           sql,
                           result,
                           true);
    return result.size() > 0;
}

bool PGUtils::pgCryptoInstalled() const
{
    PGSqlString sql("select * from pg_extension where extname = :extname");
    sql.set("extname", "pgcrypto");
    pqxx::connection c(pool.generateConnstring());
    pqxx::work w(c);
    pqxx::result result(w.exec(sql.str()));
    w.commit();
    return result.size() > 0;
}

bool PGUtils::installPGCrypto() const
{
    PGSqlString sql("CREATE EXTENSION pgcrypto");
    pqxx::connection c(pool.generateConnstring());
    pqxx::work w(c);
    pqxx::result result(w.exec(sql.str()));
    w.commit();
    return result.size() > 0;
}

bool PGUtils::roleExists(const std::string &roleName) const
{
    PGSqlString sql("select 1 from pg_roles ");
    sql += " where rolname = :roleName";
    sql.set("roleName", ExtString::lower(roleName));
    pqxx::result result;
    PGCommandTransactor ct(pool,
                           sql,
                           result);
    return result.size() > 0;
}

bool PGUtils::createRole(const std::string &name,
                         const std::string &password,
                         bool superUser) const
{
    PGSqlString sql("create role ");
    sql += name + " with login encrypted password :password ";
    if (superUser)
    {
        sql += " superuser";
    }
    sql.set("password", password);
    pqxx::result result;
    PGCommandTransactor ct(pool,
                           sql,
                           result);
    return result.size() > 0;
}

bool PGUtils::dropRole(const std::string &name) const
{
    PGSqlString sql("drop role ");
    sql += name;
    pqxx::result result;
    PGCommandTransactor ct(pool,
                           sql,
                           result);
    return result.size() > 0;
}
