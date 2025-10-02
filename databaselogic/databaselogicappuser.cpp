#include "databaselogicappuser.h"
#include "pgexecutor.h"
#include "JWUtils/definitions.h"
#include "orm_implementions/t0004_user_logintoken.h"
#include "orm-mapper/orm2postgres.h"
#include "logstatcontroller.h"
#include "JWUtils/extuuid.h"
#include "orm_implementions/t0003_user_passwordhashes.h"


DatabaseLogicAppUser::DatabaseLogicAppUser(LogStatController &logStatController,
                                           PGConnectionPool &pool,
                                           ORMPersistenceInterface &opi):
    logStatController(logStatController),
    pool(pool),
    opi(opi)
{

}



bool DatabaseLogicAppUser::searchProfiles(const reducedsole::uuid &appId,
                                          const std::string &needle,
                                          size_t limit,
                                          const size_t offset,
                                          std::string &message,
                                          rapidjson::Value &target,
                                          rapidjson::MemoryPoolAllocator<> &alloc)
{
    if (!needle.size())
    {
        message = "you must provide a needle to search for";
        return false;
    }
    if (limit == 0)
    {
        limit = 50;
    }
    std::vector<std::string> needles;
    ExtString::split(needle, " ", needles);
    SqlString sql("select id, visible_name, image_id from ");
    t0002_user userProfile;
    sql += userProfile.getORMName();
    sql.addCompare("where", "app_id", "=", appId);
    sql.addCompare("and", "verified", "is not", TimePointPostgreSqlNull);
    sql += " and ( ";
    sql += " ( searching_fuzzy_allowed ";
    int index(0);
    for (const auto &n : needles)
    {
        std::string variable(std::string("v") + ExtString::toString(index));
        sql += std::string(" and lower(visible_name) like lower(:") + variable + ") ";
        sql.set(variable, std::string("%") + n + std::string("%"));
        ++index;
    }
    sql += " ) or ( ";
    sql += " searching_exactly_allowed and visible_name = :needle ";
    sql.set("needle", needle);
    sql += " ) ) ";
    sql.limit(limit, offset);
    ORM2Postgres o2p(pool);
    o2p.toJsonArray(sql, target, alloc);
    return true;
}

