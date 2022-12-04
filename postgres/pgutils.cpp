#include "pgutils.h"
#include "pgcommandtransactor.h"
#include "utils/extstring.h"
#include "pgexecutor.h"
#include "definitions.h"

std::map<std::string, PGSqlString> PGUtils::tableName2InsertString;
std::map<std::string, PGSqlString> PGUtils::tableName2UpdateString;

PGUtils::PGUtils(PGConnectionPool &pool):pool(pool)
{

}

bool PGUtils::tableHasColumn(const std::string &tableName,
                             const std::string &columnName) const
{
    PGSqlString sql("select exists ( "
                    "select 1 "
                    "FROM information_schema.columns "
                    "WHERE table_name= :tablename and column_name= :columnname) ");
    MACRO_set(tableName);
    MACRO_set(columnName);
    PGExecutor e(pool, sql);
    return e.boolean("exists");
}

void PGUtils::alterTableAddColumn(const std::string &tableName,
                                  const std::string &columnName,
                                  const std::string &columnType) const
{
    PGSqlString sql("alter table ");
    sql += tableName;
    sql += " add column " + columnName + " " + columnType;
    PGExecutor e(pool, sql);
}

void PGUtils::alterTableAddColumnIfNeeded(const std::string &tableName,
                                          const std::string &columnName,
                                          const std::string &columnType) const
{
    if (tableHasColumn(tableName, columnName))
    {
        return;
    }
    alterTableAddColumn(tableName, columnName, columnType);
}

void PGUtils::alterTableAddColumnIfNeeded(const std::string &tableName,
                                          const PGColumnAndType &columnAndType) const
{
    alterTableAddColumnIfNeeded(tableName,
                                columnAndType.column,
                                columnAndType.typeString());
}

void PGUtils::alterTableAddColumnsIfNeeded(const std::string &tableName,
                                           const std::vector<PGColumnAndType> &columnsAndTypes) const
{
    for (const auto &cat : columnsAndTypes)
    {
        alterTableAddColumnIfNeeded(tableName, cat);
    }
}

bool PGUtils::createTable(const std::string &tableName,
                          const std::vector<PGColumnAndType> &columnsAndTypes) const
{
    PGSqlString sql("create table ");
    sql += tableName;
    sql += " ( ";
    bool first(true);
    for (const auto &cat : columnsAndTypes)
    {
        if (!first)
        {
            sql += ",";
        }
        first = false;
        sql += cat.toString();
        if (cat.primaryKey)
        {
            sql += ",";
            sql += cat.primaryKeyString();
        }
    }
    sql += " ) ";
    PGExecutor pe(pool, sql);
    for (const auto &cat : columnsAndTypes)
    {
        if (cat.index)
        {
            createIndex(tableName, tableName + "_" + cat.column, std::string("(") + cat.column + ")");
        }
    }
}

void PGUtils::createTableIfNeeded(const std::string &tableName,
                                  const std::vector<PGColumnAndType> &columnsAndTypes) const
{
    if (tableExists(tableName))
    {
        alterTableAddColumnsIfNeeded(tableName, columnsAndTypes);
        return;
    }
    createTable(tableName,
                columnsAndTypes);
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

bool PGUtils::indexExists(const std::string &tableName,
                          const std::string &indexName) const
{
    PGSqlString sql("select indexname from pg_indexes ");
    sql += "where schemaname = 'public' ";
    sql += "and tablename = :tableName "
           "and indexname = :indexName ";
    sql.set("tableName", ExtString::lower(tableName));
    sql.set("indexName", ExtString::lower(indexName));
    PGExecutor e(pool, sql);
    return e.size() > 0;
}

void PGUtils::createIndex(const std::string &tableName, const std::string &indexName, const std::string &definition) const
{
    if (indexExists(tableName, indexName))
    {
        return;
    }
    PGSqlString sql("create index ");
    sql += indexName;
    sql += " on ";
    sql += tableName;
    sql += " ";
    sql += definition;
    PGExecutor e(pool, sql);
}

PGSqlString PGUtils::createEntryExistsString(const std::string &tableName,
                                             const std::string &needleField)
{
    PGSqlString sql("select * from ");
    sql += tableName;
    sql += " where ";
    sql += ExtString::lower(needleField);
    sql += " = :";
    sql += ExtString::lower(needleField);
    sql += " limit 1";
    return sql;
}

bool PGUtils::entryExists(const std::string &tableName,
                          const std::string &needleField,
                          const std::string &needleValue)
{
    PGSqlString sql(createEntryExistsString(tableName, needleField));
    sql.set(needleField, needleValue);
    PGExecutor e(pool, sql);
    return e.size() > 0;
}

bool PGUtils::entryExists(const std::string &tableName,
                          const std::string &needleField,
                          const std::string &needleValue,
                          const std::string &needleField2,
                          const std::string &needleValue2)
{
    PGSqlString sql("select * from ");
    sql += tableName;
    sql.addCompare("where", needleField, "=", needleValue);
    sql.addCompare("and", needleField2, "=", needleValue2);
    sql += " limit 1";
    PGExecutor e(pool, sql);
    return e.size() > 0;
}

bool PGUtils::entryExists(const std::string &tableName,
                          const std::string &needleField,
                          const std::string &needleValue,
                          const std::string &needleField2,
                          const std::string &needleValue2,
                          const std::string &needleField3,
                          const std::chrono::system_clock::time_point &needleValue3)
{
    PGSqlString sql("select * from ");
    sql += tableName;
    sql.addCompare("where", needleField, "=", needleValue);
    sql.addCompare("and", needleField2, "=", needleValue2);
    sql.addCompare("and", needleField3, needleValue3 == TimePointPostgreSqlNull ? "is" : "=", needleValue3);
    sql += " limit 1";
    PGExecutor e(pool, sql);
    return e.size() > 0;

}

size_t PGUtils::countEntries(const std::string &tableName, const std::string &needleField, const std::string &needleValue)
{
    PGSqlString sql("select count(*) from ");
    sql += tableName;
    sql += " where ";
    sql += ExtString::lower(needleField);
    sql += " = :";
    sql += ExtString::lower(needleField);
    PGExecutor e(pool, sql);
    return e.get_size_t("count");
}

PGSqlString PGUtils::createInsertString(const std::string &tableName)
{
    std::map<std::string, PGSqlString>::iterator it(tableName2InsertString.find(tableName));
    if (it != tableName2InsertString.end())
    {
        return it->second;
    }
    PGSqlString sql("select * from ");
    sql += tableName;
    sql += " limit 1";
    PGExecutor e(pool, sql);

    PGSqlString insertSQL("insert into ");
    insertSQL += tableName;
    std::string fields(" (");
    std::string values(") values (:");
    fields += e.columnName(0);
    values += e.columnName(0);
    for (size_t c(1); c < e.columns(); ++c)
    {
        fields += ", ";
        values += ", :";
        fields += e.columnName(c);
        values += e.columnName(c);
    }
    values += ")";
    insertSQL += fields;
    insertSQL += values;
    tableName2InsertString[tableName] = insertSQL;
    return insertSQL;
}

PGSqlString PGUtils::createUpdateString(const std::string &tableName,
                                        const std::string &needleField)
{
    std::map<std::string, PGSqlString>::iterator it(tableName2UpdateString.find(tableName));
    if (it != tableName2UpdateString.end())
    {
        return it->second;
    }
    PGSqlString sql("select * from ");
    sql += tableName;
    sql += " limit 1";
    PGExecutor e(pool, sql);

    PGSqlString updateSQL("update ");
    updateSQL += tableName;
    updateSQL += " set ";
    updateSQL += e.columnName(0);
    updateSQL += " = :";
    updateSQL += e.columnName(0);
    for (size_t c(1); c < e.columns(); ++c)
    {
        updateSQL +=  ", ";
        updateSQL +=  e.columnName(c);
        updateSQL += " = :";
        updateSQL +=  e.columnName(c);
    }
    updateSQL += " where ";
    updateSQL += ExtString::lower(needleField);
    updateSQL += " = :";
    updateSQL += ExtString::lower(needleField);
    tableName2UpdateString[tableName] = updateSQL;
    return updateSQL;
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
