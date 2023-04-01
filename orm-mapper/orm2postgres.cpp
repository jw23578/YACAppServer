#include "orm2postgres.h"
#include "pgsqlstring.h"

ORM2Postgres::ORM2Postgres(PGConnectionPool &pool):
    pool(pool)
{

}

void ORM2Postgres::insertOrUpdate(YACBaseObject &object)
{
    assert((void("every object must have an id as primary key"), object.propertyExists(tableFields.id)));
    PGSqlString sql;
    if (object.propertyIsNull(tableFields.id))
    {
        sql.insert(object.getORMName());
        object.id.set(sole::uuid4());
    }
    else
    {
        sql.update(object.getORMName());
    }
    std::set<ORMString> propertyNames;
    object.getPropertyNames(propertyNames);
    for (const auto &pn: propertyNames)
    {
        if (pn != tableFields.id)
        {
            if (object.propertyIsNull(pn))
            {
                sql.addInsertOrSetNull(pn);
            }
            else
            {
                sql.addInsertOrSet(pn, object.getPropertyToString(pn));
            }
        }
    }
    sql.addInsertOrWhere("where", tableFields.id, "=", object.getPropertyToString(tableFields.id));
    PGExecutor e(pool, sql);
}

bool ORM2Postgres::select(const sole::uuid &id,
                          YACBaseObject &object)
{
    PGSqlString sql;
    sql.select(object.getORMName());
    sql.addCompare("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    if (e.size() != 1)
    {
        return false;
    }
    std::set<ORMString> propertyNames;
    object.getPropertyNames(propertyNames);
    for (const auto &pn: propertyNames)
    {
        if (e.isNull(pn))
        {
            object.setPropertyNull(pn, true);
        }
        else
        {
            object.setPropertyFromString(pn, e.string(pn));
        }
    }
    return true;
}

size_t ORM2Postgres::selectAll(const YACBaseObject &ghost,
                               std::set<YACBaseObject *> &target)
{
    PGSqlString sql;
    sql.select(ghost.getORMName());
    PGExecutor e(pool, sql);
    if (!e.size())
    {
        return 0;
    }
    std::set<ORMString> propertyNames;
    ghost.getPropertyNames(propertyNames);
    for (size_t i(0); i < e.size(); ++i)
    {
        YACBaseObject *object(static_cast<YACBaseObject*>(ghost.create()));
        for (const auto &pn: propertyNames)
        {
            if (e.isNull(pn))
            {
                object->setPropertyNull(pn, true);
            }
            else
            {
                object->setPropertyFromString(pn, e.string(pn));
            }
        }
        target.insert(object);
        e.next();
    }
    return e.size();
}
