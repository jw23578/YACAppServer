#include "databaselogicspaces.h"
#include "pgexecutor.h"

bool DatabaseLogicSpaces::fetchOneSpaceOnly(const sole::uuid &id, rapidjson::Value &object, rapidjson::MemoryPoolAllocator<> &alloc, std::string &message)
{
    PGExecutor e(pool);
    return e.defaultSelectToJSON(tableNames.t0024_space, id, object, alloc, message);

}

DatabaseLogicSpaces::DatabaseLogicSpaces(LogStatController &logStatController,
                                         PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool)
{

}

bool DatabaseLogicSpaces::insertOrUpdateSpace(sole::uuid &id,
                                              const sole::uuid &app_id,
                                              const std::string &name,
                                              const sole::uuid &creater_id,
                                              const bool automatic,
                                              const std::string &access_code,
                                              rapidjson::Value &object,
                                              rapidjson::MemoryPoolAllocator<> &alloc,
                                              std::string &message)
{
    PGSqlString sql;
    sql.insertOrUpdate(id, tableNames.t0024_space);
    MACRO_addInsertOrSet(sql, app_id);
    MACRO_addInsertOrSet(sql, name);
    MACRO_addInsertOrSet(sql, creater_id);
    MACRO_addInsertOrSet(sql, automatic);
    MACRO_addInsertOrSet(sql, access_code);
    sql.addInsertOrWhere("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    return fetchOneSpaceOnly(id, object, alloc, message);
}

bool DatabaseLogicSpaces::deleteSpace(const sole::uuid &id, const sole::uuid &appuser_id, std::string &message)
{
    PGSqlString sql;
    sql.update(tableNames.t0024_space);
    sql.addSet(tableFields.deleted_datetime, TimePointPostgreSqlNow);
    sql.addSet(tableFields.deleted_appuser_id, appuser_id);
    sql.addCompare("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    return true;

}

bool DatabaseLogicSpaces::fetchSpaces(const sole::uuid &app_id,
                                      const sole::uuid &appuser_id,
                                      rapidjson::Value &targetArray,
                                      rapidjson::MemoryPoolAllocator<> &alloc,
                                      std::string &message)
{
    PGSqlString sql("select * ");
    sql += std::string(", (exists(select 1 from ") + tableNames.t0025_space2appuser;
    sql.addCompare("where", tableFields.appuser_id, "=", appuser_id);
    sql.addCompare("and", tableFields.approved_datetime, "is not", TimePointPostgreSqlNull);
    sql += ") or creater_id =:appuser_id) as member ";
    sql += std::string(", (exists(select 1 from ") + tableNames.t0025_space2appuser;
    sql.addCompare("where", tableFields.appuser_id, "=", appuser_id);
    sql.addCompare("and", tableFields.denied_datetime, "is not", TimePointPostgreSqlNull);
    sql += ")) as denied ";
    sql += std::string(", (exists(select 1 from ") + tableNames.t0025_space2appuser;
    sql.addCompare("where", tableFields.appuser_id, "=", appuser_id);
    sql.addCompare("and", tableFields.requested_datetime, "is not", TimePointPostgreSqlNull);
    sql += ")) as requested ";
    sql += std::string(" from ") + tableNames.t0024_space;
    sql.addCompare("where", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    sql.addCompare("and", tableFields.app_id, "=", app_id);

    sql.set("appuser_id", appuser_id);
    PGExecutor e(pool, sql);
    e.toJsonArray(targetArray, alloc);
    return true;
}

bool DatabaseLogicSpaces::fetchSpace(const sole::uuid &space_id, rapidjson::Value &object, rapidjson::MemoryPoolAllocator<> &alloc, std::string &message)
{
    {
        PGExecutor e(pool);
        if (!e.defaultSelectToJSON(tableNames.t0024_space, space_id, object, alloc, message))
        {
            return false;
        }
    }
    {
        PGSqlString sql;
        sql.select(tableNames.t0025_space2appuser);
        sql.addCompare("where", tableFields.space_id, "=", space_id);
        PGExecutor e(pool, sql);
        rapidjson::Value member;
        member.SetArray();
        e.fill(member, alloc, tableFields.appuser_id);
        object.AddMember("member", member, alloc);
    }
    return true;

}

bool DatabaseLogicSpaces::insertOrUpdateSpace2AppUser(sole::uuid &id,
                                                      const sole::uuid &app_id,
                                                      const sole::uuid &space_id,
                                                      const sole::uuid &appuser_id,
                                                      const TimePoint &requested_datetime,
                                                      const TimePoint &approved_datetime,
                                                      const sole::uuid &approved_appuser_id,
                                                      const TimePoint &denied_datetime,
                                                      const sole::uuid &denied_appuser_id)
{
    PGSqlString sql;
    sql.insertOrUpdate(id, tableNames.t0025_space2appuser);
    MACRO_addInsertOrSet(sql, app_id);
    MACRO_addInsertOrSet(sql, space_id);
    MACRO_addInsertOrSet(sql, appuser_id);
    MACRO_addInsertOrSet(sql, requested_datetime);
    MACRO_addInsertOrSet(sql, approved_datetime);
    MACRO_addInsertOrSet(sql, approved_appuser_id);
    MACRO_addInsertOrSet(sql, denied_datetime);
    MACRO_addInsertOrSet(sql, denied_appuser_id);
    sql.addInsertOrWhere("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicSpaces::fetchSpaceRequests(const sole::uuid &app_id,
                                             rapidjson::Value &targetArray,
                                             rapidjson::MemoryPoolAllocator<> &alloc,
                                             std::string &message)
{
    PGSqlString sql;
    sql.select(tableNames.t0025_space2appuser);
    sql.addCompare("where", tableFields.app_id, "=", app_id);
    PGExecutor e(pool, sql);
    e.toJsonArray(targetArray, alloc);
    return true;
}
