#include "orm2postgres.h"
#include "pgexecutor.h"

ORM2Postgres::ORM2Postgres(PGConnectionPool &pool):
    pool(pool)
{

}

bool ORM2Postgres::select(const sole::uuid &id,
                          ORMObjectInterface &object)
{
    PGSqlString sql;
    sql.select(object.getORMName());
    sql.addCompare("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
    if (e.size() != 1)
    {
        return false;
    }
    for (const auto &pn: object.propertyNames())
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

bool ORM2Postgres::select(const sole::uuid &id,
                          ORMObjectInterface &object,
                          rapidjson::Value &target,
                          rapidjson::MemoryPoolAllocator<> &alloc)
{
    if (!select(id, object))
    {
        return false;
    }
    orm2json.toJson(object, target, alloc);
    return true;
}

size_t ORM2Postgres::selectAll(const ORMObjectInterface &ghost,
                               std::set<ORMObjectInterface *> &target)
{
    PGSqlString sql;
    sql.select(ghost.getORMName());
    PGExecutor e(pool, sql);
    if (!e.size())
    {
        return 0;
    }
    return fetchAll(e, ghost, target);
}

bool ORM2Postgres::postgres2object(const PGExecutor &e,
                                   ORMObjectInterface &target)
{
    for (const auto &pn: target.propertyNames())
    {
        if (e.isNull(pn))
        {
            target.setPropertyNull(pn, true);
        }
        else
        {
            target.setPropertyFromString(pn, e.string(pn));
        }
    }
    return true;
}

size_t ORM2Postgres::fetchAll(PGExecutor &e,
                              const ORMObjectInterface &ghost,
                              std::set<ORMObjectInterface *> &target)
{
    for (size_t i(0); i < e.size(); ++i)
    {
        ORMObjectInterface *object(ghost.create());
        postgres2object(e, *object);
        target.insert(object);
        e.next();
    }
    return e.size();
}

size_t ORM2Postgres::toJsonArray(PGSqlString &sql, const ORMObjectInterface &ghost, rapidjson::Value &targetArray, rapidjson::MemoryPoolAllocator<> &alloc)
{
    PGExecutor e(pool, sql);
    toJsonArray(e, ghost, targetArray, alloc);
}


void ORM2Postgres::insertOrUpdate(ORMObjectInterface &object)
{
    assert((void("every object must have an id as primary key"), object.propertyExists(tableFields.id)));
    PGSqlString sql;
    if (object.propertyIsNull(tableFields.id))
    {
        sql.insert(object.getORMName());
        object.setPropertyFromString(tableFields.id, sole::uuid4().str());
    }
    else
    {
        sql.update(object.getORMName());
    }
    for (const auto &pn: object.propertyNames())
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

size_t ORM2Postgres::toJsonArray(PGExecutor &e,
                                 const ORMObjectInterface &ghost,
                                 rapidjson::Value &targetArray,
                                 rapidjson::MemoryPoolAllocator<> &alloc)
{
    ORM2Postgres o2p(pool);
    for (size_t i(0); i < e.size(); ++i)
    {
        std::unique_ptr<ORMObjectInterface> object(ghost.create());
        if (o2p.postgres2object(e, *object))
        {
            orm2json.addToArray(*object, targetArray, alloc);
        }
        e.next();
    }
    return targetArray.Size();
}
