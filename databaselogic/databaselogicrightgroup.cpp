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

DatabaseLogicRightGroup::DatabaseLogicRightGroup(LogStatController &logStatController,
                                                 PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool)
{

}

bool DatabaseLogicRightGroup::insertRightGroup(const sole::uuid &id, const std::string &name, const sole::uuid &creater_id, std::string &message)
{
    PGSqlString sql;
    sql.insert(tableNames.t0021_right_group);
    MACRO_addInsert(sql, id);
    MACRO_addInsert(sql, name);
    MACRO_addInsert(sql, creater_id);
    PGExecutor e(pool, sql);
    return true;
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
        sql.addCompare("where", tableFields.right_number, "=", right_number);
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
    sql.addCompare("where", tableFields.right_number, "=", right_number);
    PGExecutor e(pool, sql);
    return true;
}
