#include "databaselogicrightgroup.h"
#include "pgexecutor.h"
#include "orm-mapper/orm2postgres.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm_implementions/t0002_user.h"
#include "orm_implementions/t0021_right_group.h"
#include "orm_implementions/t0022_right_group2user.h"

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
    sql += " from " + t0022_right_group2user().getORMName();
    sql.addCompare("where", tableFields.right_group_id, "=", right_group_id);
    sql.addCompare("and", tableFields.approved_datetime, " is not ", TimePointPostgreSqlNull);
    sql += ")";
    ORM2Postgres o2p(pool);
    o2p.toJsonArray(sql, member, alloc);
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
