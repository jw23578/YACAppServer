#include "databaselogicrightgroup.h"
#include "pgexecutor.h"

bool DatabaseLogicRightGroup::checkRightGroupCreator(const sole::uuid &id, const sole::uuid &creater_id, std::string &message)
{
    PGExecutor e(pool);
    if (!e.defaultSelect(tableNames.t0021_right_group, id, message))
    {
        return false;
    }
    if (e.uuid(tableFields.creater_id) != creater_id)
    {
        message = "you are not the creater of this right group";
        return false;
    }
    return true;
}

bool DatabaseLogicRightGroup::fetchOneRightGroup(const sole::uuid &id,
                                                 rapidjson::Value &object,
                                                 rapidjson::MemoryPoolAllocator<> &alloc,
                                                 std::string &message)
{
    PGExecutor e(pool);
    return e.defaultSelectToJSON(tableNames.t0021_right_group, id, object, alloc, message);
}

bool DatabaseLogicRightGroup::fetchIDOfOneRightGroupByName(const std::string &name, sole::uuid &id)
{
    PGSqlString sql;
    sql.select(tableNames.t0021_right_group);
    sql.addCompare("where", tableFields.name, "=", name);
    sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
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

bool DatabaseLogicRightGroup::insertRightGroup(const sole::uuid &id, const std::string &name, const sole::uuid &creater_id, rapidjson::Value &object, rapidjson::MemoryPoolAllocator<> &alloc, std::string &message)
{
    PGSqlString sql;
    sql.insert(tableNames.t0021_right_group);
    MACRO_addInsert(sql, id);
    MACRO_addInsert(sql, name);
    MACRO_addInsert(sql, creater_id);
    PGExecutor e(pool, sql);
    return fetchOneRightGroup(id, object, alloc, message);
}

bool DatabaseLogicRightGroup::updateRightGroup(const sole::uuid &id, const std::string &name, const sole::uuid &creater_id, std::string &message)
{
    if (!checkRightGroupCreator(id, creater_id, message))
    {
        return false;
    }
    PGSqlString sql;
    sql.update(tableNames.t0021_right_group);
    MACRO_addSet(sql, name);
    sql.addCompare("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicRightGroup::deleteRightGroup(const sole::uuid &id, const sole::uuid &creater_id, std::string &message)
{
    if (!checkRightGroupCreator(id, creater_id, message))
    {
        return false;
    }
    PGSqlString sql;
    sql.update(tableNames.t0021_right_group);
    sql.addSet(tableFields.deleted_datetime, TimePointPostgreSqlNow);
    sql.addCompare("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicRightGroup::fetchRightGroups(rapidjson::Value &targetArray, rapidjson::MemoryPoolAllocator<> &alloc, std::string &message)
{
    PGSqlString sql;
    sql.select(tableNames.t0021_right_group);
    sql.addCompare("where", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    PGExecutor e(pool, sql);
    e.toJsonArray(targetArray, alloc);
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

bool DatabaseLogicRightGroup::insertRight(const sole::uuid &id, const sole::uuid &right_group_id, const int right_number, std::string &message)
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

bool DatabaseLogicRightGroup::insertUser(const sole::uuid &id, const sole::uuid &right_group_id, const sole::uuid &appuser_id, std::string &message)
{
    {
        PGSqlString sql;
        sql.select(tableNames.t0022_right_group2appuser);
        sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
        sql.addCompare("and", tableFields.appuser_id, "=", appuser_id);
        PGExecutor e(pool, sql);
        if (e.size() > 0)
        {
            return true;
        }

    }
    PGSqlString sql;
    sql.insert(tableNames.t0022_right_group2appuser);
    MACRO_addInsert(sql, id);
    MACRO_addInsert(sql, right_group_id);
    MACRO_addInsert(sql, appuser_id);
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

void DatabaseLogicRightGroup::checkAndGenerateAdminGroup(const std::string &adminGroupName, const std::set<int> &right_numbers)
{
    sole::uuid id(sole::uuid4());
    if (!fetchIDOfOneRightGroupByName(adminGroupName, id))
    {
        PGSqlString sql;
        sql.insert(tableNames.t0021_right_group);
        MACRO_addInsert(sql, id);
        sql.addInsert(tableFields.name, adminGroupName);
        sql.addInsert(tableFields.creater_id, NullUuid);
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

bool DatabaseLogicRightGroup::adminExists(const std::string &adminGroupName)
{
    PGSqlString sql;
    sql.select(tableNames.t0022_right_group2appuser);
    sql += std::string(" where ") + tableFields.right_group_id + std::string(" = (");
    sql += std::string("select id from ") + tableNames.t0021_right_group;
    sql.addCompare("where", tableFields.name, "=", adminGroupName);
    sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    sql += std::string(" ) limit 1");
    PGExecutor e(pool, sql);
    return e.size() > 0;
}
