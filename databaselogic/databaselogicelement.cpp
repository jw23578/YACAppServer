#include "databaselogicelement.h"
#include "orm_implementions/sqlstring.h"
#include "pgexecutor.h"

DatabaseLogicElement::DatabaseLogicElement(LogStatController &logStatController,
                                           PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool)
{

}

bool DatabaseLogicElement::insertElementUsable4AppUser(const sole::uuid &id,
                                                       const sole::uuid &element_id,
                                                       const sole::uuid &appuser_id)
{
    SqlString sql;
    sql.insert(tableNames.t0017_element_usable4appuser);
    MACRO_addInsert(sql, id);
    MACRO_addInsert(sql, element_id);
    MACRO_addInsert(sql, appuser_id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicElement::updateElementUsable4AppUser(const sole::uuid &id,
                                                       const sole::uuid &element_id,
                                                       const sole::uuid &appuser_id)
{
    SqlString sql;
    sql.update(tableNames.t0017_element_usable4appuser);
    MACRO_addSet(sql, element_id);
    MACRO_addSet(sql, appuser_id);
    sql.addCompare(" where ", tableFields.id, " = ", id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicElement::deleteElementUsable4AppUser(const sole::uuid &id)
{
    PGExecutor e(pool);
    e.defaultDelete(tableNames.t0017_element_usable4appuser, id);
    return true;
}

bool DatabaseLogicElement::insertElementVisible4AppUser(const sole::uuid &id,
                                                        const sole::uuid &element_id,
                                                        const sole::uuid &appuser_id)
{
    SqlString sql;
    sql.insert(tableNames.t0019_element_visible4appuser);
    MACRO_addInsert(sql, id);
    MACRO_addInsert(sql, element_id);
    MACRO_addInsert(sql, appuser_id);
    PGExecutor e(pool, sql);
    return true;

}

bool DatabaseLogicElement::updateElementVisible4AppUser(const sole::uuid &id,
                                                        const sole::uuid &element_id,
                                                        const sole::uuid &appuser_id)
{
    SqlString sql;
    sql.update(tableNames.t0019_element_visible4appuser);
    MACRO_addSet(sql, element_id);
    MACRO_addSet(sql, appuser_id);
    sql.addCompare(" where ", tableFields.id, " = ", id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicElement::deleteElementVisible4AppUser(const sole::uuid &id)
{
    PGExecutor e(pool);
    e.defaultDelete(tableNames.t0019_element_visible4appuser, id);
    return true;
}
