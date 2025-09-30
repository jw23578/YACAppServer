#include "databaselogicrightgroup.h"
#include "pgexecutor.h"
#include "orm-mapper/orm2postgres.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm_implementions/t0002_user.h"
#include "orm_implementions/t0021_right_group.h"

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

DatabaseLogicRightGroup::DatabaseLogicRightGroup(LogStatController &logStatController,
                                                 PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool)
{

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
