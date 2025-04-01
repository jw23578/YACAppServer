#include "databaselogicthirdparty.h"
#include "pgexecutor.h"

DatabaseLogicThirdParty::DatabaseLogicThirdParty(LogStatController &logStatController,
                                                 PGConnectionPool &pool):
    DatabaseLogicInterface(logStatController, pool)
{

}

bool DatabaseLogicThirdParty::create(const reducedsole::uuid &app_id,
                                     const std::string &third,
                                     const std::string &mandant,
                                     const std::string &user_id_string,
                                     const std::string &image_id_string,
                                     const reducedsole::uuid &appuser_id,
                                     t0029_third_party_user_data &result)
{
    std::string third_party_string(third + mandant);
    result.app_id = app_id;
    result.third_party_string = third_party_string;
    result.user_id_string = user_id_string;
    result.image_id_string = image_id_string;
    result.appuser_id = appuser_id;
    orm2postgres.insert(result);
    return true;
}

bool DatabaseLogicThirdParty::update(const t0029_third_party_user_data &user)
{
    orm2postgres.update(user);
    return true;
}

bool DatabaseLogicThirdParty::lookup(const reducedsole::uuid app_id,
                                     const std::string &third,
                                     const std::string &mandant,
                                     const std::string &user_id_string,
                                     t0029_third_party_user_data &result)
{
    std::string third_party_string(third + mandant);
    SqlString sql;
    sql.select(result.getORMName());
    sql.addCompare("where", tableFields.app_id, "=", app_id);
    sql.addCompare("and", tableFields.third_party_string, "=", third_party_string);
    sql.addCompare("and", tableFields.user_id_string, "=", user_id_string);
    PGExecutor e(pool, sql);
    return orm2postgres.postgres2object(e, result);
}
