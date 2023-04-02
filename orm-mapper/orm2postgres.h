#ifndef ORM2POSTGRES_H
#define ORM2POSTGRES_H

#include "pgconnectionpool.h"
#include "orm/ormobjectinterface.h"
#include "tablefields.h"
#include "orm2rapidjson.h"
#include "pgsqlstring.h"

class PGExecutor;

class ORM2Postgres
{
    PGConnectionPool &pool;
    const TableFields tableFields;
    ORM2rapidjson orm2json;
public:
    ORM2Postgres(PGConnectionPool &pool);

    void insertOrUpdate(ORMObjectInterface &object);

    bool select(const sole::uuid &id,
                ORMObjectInterface &object);
    bool select(const sole::uuid &id,
                ORMObjectInterface &object,
                rapidjson::Value &target,
                rapidjson::MemoryPoolAllocator<> &alloc);

    size_t selectAll(const ORMObjectInterface &ghost,
                     std::set<ORMObjectInterface*> &target);

    template <class NeedleValueType1>
    size_t selectAll(const ORMObjectInterface &ghost,
                     const std::string &needle1,
                     NeedleValueType1 needleValue1,
                     rapidjson::Value &targetArray,
                     rapidjson::MemoryPoolAllocator<> &alloc)
    {
        PGSqlString sql;
        sql.select(ghost.getORMName());
        sql.addCompare("where", needle1, "=", needleValue1);
        toJsonArray(sql, ghost, targetArray, alloc);
    }

    bool postgres2object(const PGExecutor &e,
                         ORMObjectInterface &target);

    size_t fetchAll(PGExecutor &e,
                    const ORMObjectInterface &ghost,
                    std::set<ORMObjectInterface*> &target);

    size_t toJsonArray(PGSqlString &sql,
                       const ORMObjectInterface &ghost,
                       rapidjson::Value &targetArray,
                       rapidjson::MemoryPoolAllocator<> &alloc);

    size_t toJsonArray(PGExecutor &e,
                       const ORMObjectInterface &ghost,
                       rapidjson::Value &targetArray,
                       rapidjson::MemoryPoolAllocator<> &alloc);
};

#endif // ORM2POSTGRES_H
