#include "databaselogicrightgroup.h"
#include "pgexecutor.h"
#include "orm-mapper/orm2postgres.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm_implementions/t0021_right_group.h"

bool DatabaseLogicRightGroup::fetchOneRightGroup(const sole::uuid &id,
                                                 rapidjson::Value &object,
                                                 rapidjson::MemoryPoolAllocator<> &alloc,
                                                 std::string &message)
{
    PGExecutor e(pool);
    return e.defaultSelectToJSON(tableNames.t0021_right_group, id, object, alloc, message);
}

bool DatabaseLogicRightGroup::appuserInRightGroup(const sole::uuid &right_group_id, const sole::uuid &appuser_id)
{
    PGSqlString sql;
    sql.select(tableNames.t0022_right_group2appuser);
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    sql.addCompare("and", tableFields.appuser_id, "=", appuser_id);
    sql.addCompare("and", tableFields.approved_datetime, "is not", TimePointPostgreSqlNull);
    PGExecutor e(pool, sql);
    return e.size() > 0;
}

bool DatabaseLogicRightGroup::fetchIDOfOneRightGroupByName(const sole::uuid &app_id, const std::string &name, sole::uuid &id)
{
    PGSqlString sql;
    sql.select(tableNames.t0021_right_group);
    sql.addCompare("where", tableFields.name, "=", name);
    sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    sql.addCompare("and", tableFields.app_id, "=", app_id);
    PGExecutor e(pool, sql);
    if (!e.size())
    {
        return false;
    }
    id = e.uuid(tableFields.id);
    return true;
}

DatabaseLogicRightGroup::DatabaseLogicRightGroup(LogStatController &logStatController,
                                                 PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool)
{

}

bool DatabaseLogicRightGroup::deleteRightGroup(const sole::uuid &id, const sole::uuid &appuser_id, std::string &message)
{
    PGSqlString sql;
    sql.update(tableNames.t0021_right_group);
    sql.addSet(tableFields.deleted_datetime, TimePointPostgreSqlNow);
    sql.addSet(tableFields.deleted_appuser_id, appuser_id);
    sql.addCompare("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicRightGroup::fetchRightGroup(const sole::uuid &right_group_id,
                                              rapidjson::Value &object,
                                              rapidjson::MemoryPoolAllocator<> &alloc,
                                              std::string &message)
{
    ORM2Postgres orm2postgres(pool);
    t0021_right_group t0021;
    if (!orm2postgres.select(right_group_id, t0021, object, alloc))
    {
        return false;
    }
//    {
//        PGExecutor e(pool);
//        if (!e.defaultSelectToJSON(tableNames.t0021_right_group, right_group_id, object, alloc, message))
//        {
//            return false;
//        }
//    }
    {
        PGSqlString sql;
        sql.select(tableNames.t0023_right2rightgroup);
        sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
        PGExecutor e(pool, sql);
        rapidjson::Value rightNumbers;
        rightNumbers.SetArray();
        e.fill(rightNumbers, alloc, tableFields.right_number);
        object.AddMember("rightNumbers", rightNumbers, alloc);
    }
    {
        PGSqlString sql;
        sql.select(tableNames.t0022_right_group2appuser);
        sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
        PGExecutor e(pool, sql);
        rapidjson::Value member;
        member.SetArray();
        e.fill(member, alloc, tableFields.appuser_id);
        object.AddMember("member", member, alloc);
    }
    return true;
}

bool DatabaseLogicRightGroup::fetchRightGroupMember(const sole::uuid &right_group_id,
                                                    rapidjson::Value &member,
                                                    rapidjson::MemoryPoolAllocator<> &alloc,
                                                    std::string &errorMessage)
{
    PGSqlString sql("select id, visible_name, image_id from ");
    sql += tableNames.t0003_appuser_profiles;
    sql += " where " + tableFields.id + " in (select " + tableFields.appuser_id;
    sql += " from " + tableNames.t0022_right_group2appuser;
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    sql.addCompare("and", tableFields.approved_datetime, " is not ", TimePointPostgreSqlNull);
    sql += ")";
    PGExecutor e(pool, sql);
    e.deprecated_toJsonArray(member, alloc);
    return true;
}

void DatabaseLogicRightGroup::fetchAppUserRightNumbers(const sole::uuid &appuser_id, std::set<int> &right_numbers)
{
    PGSqlString sql;
    sql.select(tableNames.t0023_right2rightgroup);
    sql += std::string(" where ") + tableFields.right_group_id;
    sql += std::string(" in (select ") + tableFields.right_group_id;
    sql += std::string(" from ") + tableNames.t0022_right_group2appuser;
    sql.addCompare("where", tableFields.appuser_id, "=", appuser_id);
    sql += std::string(")");
    PGExecutor e(pool, sql);
    for (size_t i(0); i < e.size(); ++i)
    {
        right_numbers.insert(e.integer(tableFields.right_number));
        e.next();
    }
}

bool DatabaseLogicRightGroup::insertRight(const sole::uuid &id,
                                          const sole::uuid &right_group_id,
                                          const int right_number,
                                          std::string &message)
{
    {
        PGSqlString sql;
        sql.select(tableNames.t0023_right2rightgroup);
        sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
        sql.addCompare("and", tableFields.right_number, "=", right_number);
        PGExecutor e(pool, sql);
        if (e.size() > 0)
        {
            return true;
        }

    }
    PGSqlString sql;
    sql.insert(tableNames.t0023_right2rightgroup);
    MACRO_addInsert(sql, id);
    MACRO_addInsert(sql, right_group_id);
    MACRO_addInsert(sql, right_number);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicRightGroup::removeRight(const sole::uuid &right_group_id, const int right_number, std::string &message)
{
    PGSqlString sql;
    sql.delet(tableNames.t0023_right2rightgroup);
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    sql.addCompare("and", tableFields.right_number, "=", right_number);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicRightGroup::removeUser(const sole::uuid &right_group_id, const sole::uuid &appuser_id, std::string &message)
{
    PGSqlString sql;
    sql.delet(tableNames.t0022_right_group2appuser);
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    sql.addCompare("and", tableFields.appuser_id, "=", appuser_id);
    PGExecutor e(pool, sql);
    return true;
}

void DatabaseLogicRightGroup::fetchGroupRightNumbers(const sole::uuid &right_group_id,
                                                     std::set<int> &right_numbers)
{
    PGSqlString sql;
    sql.select(tableNames.t0023_right2rightgroup);
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    PGExecutor e(pool, sql);
    e.fill(right_numbers, tableFields.right_number);
}

void DatabaseLogicRightGroup::checkAndGenerateAdminGroup(const sole::uuid &app_id,
                                                         const std::string &adminGroupName, const std::set<int> &right_numbers)
{
    sole::uuid id(sole::uuid4());
    if (!fetchIDOfOneRightGroupByName(app_id, adminGroupName, id))
    {
        PGSqlString sql;
        sql.insert(tableNames.t0021_right_group);
        MACRO_addInsert(sql, id);
        sql.addInsert(tableFields.name, adminGroupName);
        sql.addInsert(tableFields.creater_id, NullUuid);
        sql.addInsert(tableFields.app_id, app_id);
        PGExecutor e(pool, sql);
    }
    std::string message;
    std::set<int> current_right_numbers;
    fetchGroupRightNumbers(id, current_right_numbers);
    for (const auto &rn: right_numbers)
    {
        if (current_right_numbers.find(rn) == current_right_numbers.end())
        {
            insertRight(sole::uuid4(), id, rn, message);
        }
    }
}

bool DatabaseLogicRightGroup::adminExists(const sole::uuid &app_id, const std::string &adminGroupName)
{
    PGSqlString sql;
    sql.select(tableNames.t0022_right_group2appuser);
    sql += std::string(" where ") + tableFields.right_group_id + std::string(" = (");
    sql += std::string("select id from ") + tableNames.t0021_right_group;
    sql.addCompare("where", tableFields.name, "=", adminGroupName);
    sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    sql.addCompare("and", tableFields.app_id, "=", app_id);
    sql += std::string(" ) limit 1");
    PGExecutor e(pool, sql);
    return e.size() > 0;
}
