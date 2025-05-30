#include "databaselogicspaces.h"
#include "pgexecutor.h"
#include "orm-mapper/orm2postgres.h"

bool DatabaseLogicSpaces::fetchOneSpaceOnly(const reducedsole::uuid &id, rapidjson::Value &object, rapidjson::MemoryPoolAllocator<> &alloc, std::string &message)
{
    ORM2Postgres o2p(pool);
    return o2p.defaultSelectToJSON(tableNames.t0024_space, id, object, alloc, message);

}

DatabaseLogicSpaces::DatabaseLogicSpaces(LogStatController &logStatController,
                                         PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool)
{

}

bool DatabaseLogicSpaces::insertOrUpdateSpace(reducedsole::uuid &id,
                                              const reducedsole::uuid &app_id,
                                              const std::string &name,
                                              const reducedsole::uuid &creater_id,
                                              const bool automatic,
                                              const std::string &access_code,
                                              const bool request_allowed,
                                              rapidjson::Value &object,
                                              rapidjson::MemoryPoolAllocator<> &alloc,
                                              std::string &message)
{
    SqlString sql;
    sql.insertOrUpdate(id, tableNames.t0024_space);
    MACRO_addInsertOrSet(sql, app_id);
    MACRO_addInsertOrSet(sql, name);
    MACRO_addInsertOrSet(sql, creater_id);
    MACRO_addInsertOrSet(sql, automatic);
    MACRO_addInsertOrSet(sql, access_code);
    MACRO_addInsertOrSet(sql, request_allowed);
    sql.addInsertOrWhere("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    return fetchOneSpaceOnly(id, object, alloc, message);
}

bool DatabaseLogicSpaces::spaceRequestResultSeen(const reducedsole::uuid &id,
                                                 const reducedsole::uuid &appuser_id,
                                                 std::string &errorMessage)
{
    SqlString sql;
    sql.update(tableNames.t0025_space2appuser);
    sql.addSet(tableFields.result_seen, TimePointPostgreSqlNow, false);
    sql.addCompare("where", tableFields.id, "=", id);
    sql.addCompare("and", tableFields.appuser_id, "=", appuser_id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicSpaces::deleteSpace(const reducedsole::uuid &id, const reducedsole::uuid &appuser_id, std::string &message)
{
    SqlString sql;
    sql.update(tableNames.t0024_space);
    sql.addSet(tableFields.deleted_datetime, TimePointPostgreSqlNow, false);
    sql.addSet(tableFields.deleted_appuser_id, appuser_id, false);
    sql.addCompare("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    return true;

}

bool DatabaseLogicSpaces::fetchSpaces(const reducedsole::uuid &app_id,
                                      const reducedsole::uuid &appuser_id,
                                      rapidjson::Value &targetArray,
                                      rapidjson::MemoryPoolAllocator<> &alloc,
                                      std::string &message)
{
    SqlString sql("select * ");
    sql += std::string(", (exists(select 1 from ") + tableNames.t0025_space2appuser;
    sql.addCompare("where", tableFields.appuser_id, "=", appuser_id);
    sql.addCompare("and", tableFields.approved_datetime, "is not", TimePointPostgreSqlNull);
    sql += std::string(" and ") + tableFields.space_id + " = t0024.id ";
    sql += ") or creater_id =:appuser_id) as member ";
    sql += std::string(", (exists(select 1 from ") + tableNames.t0025_space2appuser;
    sql.addCompare("where", tableFields.appuser_id, "=", appuser_id);
    sql.addCompare("and", tableFields.denied_datetime, "is not", TimePointPostgreSqlNull);
    sql += std::string(" and ") + tableFields.space_id + " = t0024.id ";
    sql += ")) as denied ";
    sql += std::string(", (exists(select 1 from ") + tableNames.t0025_space2appuser;
    sql.addCompare("where", tableFields.appuser_id, "=", appuser_id);
    sql.addCompare("and", tableFields.requested_datetime, "is not", TimePointPostgreSqlNull);
    sql += std::string(" and ") + tableFields.space_id + " = t0024.id ";
    sql += ")) as requested ";
    sql += std::string(" from ") + tableNames.t0024_space + std::string(" as t0024 ");
    sql.addCompare("where", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    sql.addCompare("and", tableFields.app_id, "=", app_id);

    sql.set("appuser_id", appuser_id);
    ORM2Postgres o2p(pool);
    o2p.toJsonArray(sql, targetArray, alloc);
    return true;
}

bool DatabaseLogicSpaces::fetchSpace(const reducedsole::uuid &space_id, rapidjson::Value &object, rapidjson::MemoryPoolAllocator<> &alloc, std::string &message)
{
    {
        ORM2Postgres o2p(pool);
        if (!o2p.defaultSelectToJSON(tableNames.t0024_space, space_id, object, alloc, message))
        {
            return false;
        }
    }
    {
        SqlString sql;
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

bool DatabaseLogicSpaces::insertOrUpdateSpace2AppUser(reducedsole::uuid &id,
                                                      const reducedsole::uuid &app_id,
                                                      const reducedsole::uuid &space_id,
                                                      const reducedsole::uuid &appuser_id,
                                                      const TimePoint &requested_datetime,
                                                      const TimePoint &approved_datetime,
                                                      const reducedsole::uuid &approved_appuser_id,
                                                      const TimePoint &denied_datetime,
                                                      const reducedsole::uuid &denied_appuser_id)
{
    SqlString sql;
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

bool DatabaseLogicSpaces::fetchSpaceRequests(const reducedsole::uuid &spaceAdminId,
                                             rapidjson::Value &targetArray,
                                             rapidjson::MemoryPoolAllocator<> &alloc,
                                             std::string &errorMessage)
{
    SqlString sql("select t0025.id, t0025.space_id, t0025.appuser_id, t0024.name ");
    sql += " from " + tableNames.t0025_space2appuser + " t0025 ";
    sql += " left join " + tableNames.t0024_space + " t0024 on t0024.id = t0025.space_id ";
    sql.addCompare("where", tableFields.requested_datetime, " is not ", TimePointPostgreSqlNull);
    sql.addCompare("and", tableFields.approved_datetime, " is ", TimePointPostgreSqlNull);

    sql += " and " + tableFields.space_id + " in (select id from " + tableNames.t0024_space;
    sql.addCompare("where", tableFields.creater_id, "=", spaceAdminId);
    sql += ")";

    ORM2Postgres o2p(pool);
    o2p.toJsonArray(sql, targetArray, alloc);
    return true;
}

bool DatabaseLogicSpaces::fetchSpaceRequestId(const reducedsole::uuid &space_id, const reducedsole::uuid &appuser_id, reducedsole::uuid &id)
{
    SqlString sql;
    sql.select(tableNames.t0025_space2appuser);
    sql.addCompare("where", tableFields.space_id, "=", space_id);
    sql.addCompare("and", tableFields.appuser_id, "=", appuser_id);
    PGExecutor e(pool, sql);
    if (!e.size())
    {
        return false;
    }
    id = e.uuid(tableFields.id);
    return true;
}
