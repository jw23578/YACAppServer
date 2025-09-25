#include "databaselogicrightgroup.h"
#include "pgexecutor.h"
#include "orm-mapper/orm2postgres.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm_implementions/t0002_user.h"
#include "orm_implementions/t0021_right_group.h"

bool DatabaseLogicRightGroup::fetchOneRightGroup(const reducedsole::uuid &id,
                                                 rapidjson::Value &object,
                                                 rapidjson::MemoryPoolAllocator<> &alloc,
                                                 std::string &message)
{
    ORM2Postgres o2p(pool);
    return o2p.defaultSelectToJSON(tableNames.t0021_right_group, id, object, alloc, message);
}

bool DatabaseLogicRightGroup::appuserInRightGroup(const reducedsole::uuid &right_group_id, const reducedsole::uuid &appuser_id)
{
    SqlString sql;
    sql.select(tableNames.t0022_right_group2appuser);
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    sql.addCompare("and", tableFields.user_id, "=", appuser_id);
    sql.addCompare("and", tableFields.approved_datetime, "is not", TimePointPostgreSqlNull);
    PGExecutor e(pool, sql);
    return e.size() > 0;
}

bool DatabaseLogicRightGroup::fetchIDOfOneRightGroupByName(const reducedsole::uuid &app_id, const std::string &name, reducedsole::uuid &id)
{
    SqlString sql;
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

bool DatabaseLogicRightGroup::deleteRightGroup(const reducedsole::uuid &id, const reducedsole::uuid &appuser_id, std::string &message)
{
    SqlString sql;
    sql.update(tableNames.t0021_right_group);
    sql.addSet(tableFields.deleted_datetime, TimePointPostgreSqlNow, false);
    sql.addSet(tableFields.deleted_appuser_id, appuser_id, false);
    sql.addCompare("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicRightGroup::fetchRightGroup(const reducedsole::uuid &right_group_id,
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
//        ORM2Postgres o2p(pool);
//        if (!o2p.defaultSelectToJSON(tableNames.t0021_right_group, right_group_id, object, alloc, message))
//        {
//            return false;
//        }
//    }
    {
        SqlString sql;
        sql.select(tableNames.t0023_right2rightgroup);
        sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
        PGExecutor e(pool, sql);
        rapidjson::Value rightNumbers;
        rightNumbers.SetArray();
        e.fill(rightNumbers, alloc, tableFields.right_number);
        object.AddMember("rightNumbers", rightNumbers, alloc);
    }
    {
        SqlString sql;
        sql.select(tableNames.t0022_right_group2appuser);
        sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
        PGExecutor e(pool, sql);
        rapidjson::Value member;
        member.SetArray();
        e.fill(member, alloc, tableFields.user_id);
        object.AddMember("member", member, alloc);
    }
    return true;
}

bool DatabaseLogicRightGroup::fetchRightGroupMember(const reducedsole::uuid &right_group_id,
                                                    rapidjson::Value &member,
                                                    rapidjson::MemoryPoolAllocator<> &alloc,
                                                    std::string &errorMessage)
{
    t0002_user userProfile;
    SqlString sql("select id, visible_name, image_id from ");
    sql += userProfile.getORMName();
    sql += " where " + tableFields.id + " in (select " + tableFields.user_id;
    sql += " from " + tableNames.t0022_right_group2appuser;
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    sql.addCompare("and", tableFields.approved_datetime, " is not ", TimePointPostgreSqlNull);
    sql += ")";
    ORM2Postgres o2p(pool);
    o2p.toJsonArray(sql, member, alloc);
    return true;
}

void DatabaseLogicRightGroup::fetchAppUserRightNumbers(const reducedsole::uuid &appuser_id, std::set<int> &right_numbers)
{
    SqlString sql;
    sql.select(tableNames.t0023_right2rightgroup);
    sql += std::string(" where ") + tableFields.right_group_id;
    sql += std::string(" in (select ") + tableFields.right_group_id;
    sql += std::string(" from ") + tableNames.t0022_right_group2appuser;
    sql.addCompare("where", tableFields.user_id, "=", appuser_id);
    sql += std::string(")");
    PGExecutor e(pool, sql);
    for (size_t i(0); i < e.size(); ++i)
    {
        right_numbers.insert(e.integer(tableFields.right_number));
        e.next();
    }
}

bool DatabaseLogicRightGroup::insertRight(const reducedsole::uuid &id,
                                          const reducedsole::uuid &right_group_id,
                                          const int right_number,
                                          std::string &message)
{
    {
        SqlString sql;
        sql.select(tableNames.t0023_right2rightgroup);
        sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
        sql.addCompare("and", tableFields.right_number, "=", right_number);
        PGExecutor e(pool, sql);
        if (e.size() > 0)
        {
            return true;
        }

    }
    SqlString sql;
    sql.insert(tableNames.t0023_right2rightgroup);
    MACRO_addInsert(sql, id);
    MACRO_addInsert(sql, right_group_id);
    MACRO_addInsert(sql, right_number);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicRightGroup::removeRight(const reducedsole::uuid &right_group_id, const int right_number, std::string &message)
{
    SqlString sql;
    sql.delet(tableNames.t0023_right2rightgroup);
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    sql.addCompare("and", tableFields.right_number, "=", right_number);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicRightGroup::removeUser(const reducedsole::uuid &right_group_id, const reducedsole::uuid &appuser_id, std::string &message)
{
    SqlString sql;
    sql.delet(tableNames.t0022_right_group2appuser);
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    sql.addCompare("and", tableFields.user_id, "=", appuser_id);
    PGExecutor e(pool, sql);
    return true;
}

void DatabaseLogicRightGroup::fetchGroupRightNumbers(const reducedsole::uuid &right_group_id,
                                                     std::set<int> &right_numbers)
{
    SqlString sql;
    sql.select(tableNames.t0023_right2rightgroup);
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    PGExecutor e(pool, sql);
    e.fill(right_numbers, tableFields.right_number);
}

void DatabaseLogicRightGroup::checkAndGenerateAdminGroup(const reducedsole::uuid &app_id,
                                                         const std::string &adminGroupName, const std::set<int> &right_numbers)
{
    reducedsole::uuid id(reducedsole::uuid4());
    if (!fetchIDOfOneRightGroupByName(app_id, adminGroupName, id))
    {
        SqlString sql;
        sql.insert(tableNames.t0021_right_group);
        MACRO_addInsert(sql, id);
        sql.addInsert(tableFields.name, adminGroupName, false);
        sql.addInsert(tableFields.creater_id, ExtUuid::NullUuid, false);
        sql.addInsert(tableFields.app_id, app_id, false);
        PGExecutor e(pool, sql);
    }
    std::string message;
    std::set<int> current_right_numbers;
    fetchGroupRightNumbers(id, current_right_numbers);
    for (const auto &rn: right_numbers)
    {
        if (current_right_numbers.find(rn) == current_right_numbers.end())
        {
            insertRight(reducedsole::uuid4(), id, rn, message);
        }
    }
}

bool DatabaseLogicRightGroup::adminExists(const reducedsole::uuid &app_id, const std::string &adminGroupName)
{
    SqlString sql;
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
