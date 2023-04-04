#include "orm2postgres.h"
#include "pgexecutor.h"

ORM2Postgres::ORM2Postgres(PGConnectionPool &pool):
    pool(pool)
{

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
    for (const auto &pn: object.propertyNames())
    {
        if (!object.isTransferProperty(pn))
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
    }
    return true;
}

bool ORM2Postgres::select(const sole::uuid &id,
                          YACBaseObject &object,
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
    return fetchAll(e, ghost, target);
}

bool ORM2Postgres::postgres2object(const PGExecutor &e,
                                   YACBaseObject &target)
{
    for (const auto &pn: target.propertyNames())
    {
        if (!target.isTransferProperty(pn))
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
    }
    return true;
}

size_t ORM2Postgres::fetchAll(PGExecutor &e,
                              const YACBaseObject &ghost,
                              std::set<YACBaseObject *> &target)
{
    for (size_t i(0); i < e.size(); ++i)
    {
        YACBaseObject *object(static_cast<YACBaseObject*>(ghost.create()));
        postgres2object(e, *object);
        target.insert(object);
        e.next();
    }
    return e.size();
}

size_t ORM2Postgres::toJsonArray(PGSqlString &sql, const YACBaseObject &ghost, rapidjson::Value &targetArray, rapidjson::MemoryPoolAllocator<> &alloc)
{
    PGExecutor e(pool, sql);
    return toJsonArray(e, ghost, targetArray, alloc);
}

void ORM2Postgres::insert(YACBaseObject &object)
{
    assert((void("every object must have an id as primary key"), object.propertyExists(tableFields.id)));
    PGSqlString sql;
    sql.insert(object.getORMName());
    if (object.propertyIsNull(tableFields.id))
    {
        object.setPropertyFromString(tableFields.id, sole::uuid4().str());
    }
    for (const auto &pn: object.propertyNames())
    {
        if (pn != tableFields.id && !object.isTransferProperty(pn))
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

void ORM2Postgres::update(YACBaseObject &object)
{
    assert((void("every object must have an id as primary key"), object.propertyExists(tableFields.id)));
    PGSqlString sql;
    if (object.propertyIsNull(tableFields.id))
    {
        assert((void("update with id == null is not possible"), object.propertyExists(tableFields.id)));
    }
    sql.update(object.getORMName());
    for (const auto &pn: object.propertyNames())
    {
        if (pn != tableFields.id && !object.isTransferProperty(pn))
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

void ORM2Postgres::insertOrUpdate(YACBaseObject &object)
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
        if (pn != tableFields.id && !object.isTransferProperty(pn))
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
                                 const YACBaseObject &ghost,
                                 rapidjson::Value &targetArray,
                                 rapidjson::MemoryPoolAllocator<> &alloc)
{
    targetArray.SetArray();
    ORM2Postgres o2p(pool);
    for (size_t i(0); i < e.size(); ++i)
    {
        std::unique_ptr<YACBaseObject> object(static_cast<YACBaseObject*>(ghost.create()));
        if (o2p.postgres2object(e, *object))
        {
            orm2json.addToArray(*object, targetArray, alloc);
        }
        e.next();
    }
    return targetArray.Size();
}
