#ifndef ORM2POSTGRES_H
#define ORM2POSTGRES_H

#include "pgconnectionpool.h"
#include "orm_implementions/yacbaseobject.h"
#include "orm/ormmap.h"
#include "tablefields.h"
#include "orm2rapidjson.h"
#include "orm_implementions/sqlstring.h"

class PGExecutor;

class ORM2Postgres
{
    PGConnectionPool &pool;
    const TableFields tableFields;
    ORM2rapidjson orm2json;
public:
    ORM2Postgres(PGConnectionPool &pool);

    void insertOrUpdate(YACBaseObject &object);

    bool select(const sole::uuid &id,
                YACBaseObject &object);
    bool select(const sole::uuid &id,
                YACBaseObject &object,
                rapidjson::Value &target,
                rapidjson::MemoryPoolAllocator<> &alloc);

    size_t selectAll(const YACBaseObject &ghost,
                     std::set<YACBaseObject*> &target);

    size_t fetchAll(PGExecutor &e,
                    const YACBaseObject &ghost,
                    ORMMap &target);

    size_t selectAll(const YACBaseObject &ghost,
                     ORMMap &target);

    template <class NeedleValueType1>
    size_t selectAll(const YACBaseObject &ghost,
                     const std::string &needle1,
                     NeedleValueType1 needleValue1,
                     rapidjson::Value &targetArray,
                     rapidjson::MemoryPoolAllocator<> &alloc)
    {
        SqlString sql;
        sql.select(ghost.getORMName());
        sql.addCompare("where", needle1, "=", needleValue1);
        toJsonArray(sql, ghost, targetArray, alloc);
        return targetArray.Size();
    }

    bool postgres2object(const PGExecutor &e,
                         YACBaseObject &target);

    size_t fetchAll(PGExecutor &e,
                    const YACBaseObject &ghost,
                    std::set<YACBaseObject*> &target);

    size_t toJsonArray(SqlString &sql,
                       const YACBaseObject &ghost,
                       rapidjson::Value &targetArray,
                       rapidjson::MemoryPoolAllocator<> &alloc);

    size_t toJsonArray(PGExecutor &e,
                       const YACBaseObject &ghost,
                       rapidjson::Value &targetArray,
                       rapidjson::MemoryPoolAllocator<> &alloc);
    void insert(YACBaseObject &object);
    void update(const YACBaseObject &object);


    void toJsonObject(PGExecutor &e,
                      rapidjson::Value &object,
                      rapidjson::MemoryPoolAllocator<> &alloc,
                      const std::set<std::string> fields2Ignore = {});
    size_t toJsonArray(PGExecutor &e,
                       rapidjson::Value &targetArray,
                       rapidjson::MemoryPoolAllocator<> &alloc,
                       const std::set<std::string> fields2Ignore = {});
    size_t toJsonArray(SqlString &sql,
                       rapidjson::Value &targetArray,
                       rapidjson::MemoryPoolAllocator<> &alloc,
                       const std::set<std::string> fields2Ignore = {});

    bool defaultSelectToJSON(const std::string &tableName,
                             const sole::uuid &id,
                             rapidjson::Value &object,
                             rapidjson::MemoryPoolAllocator<> &alloc,
                             std::string &message);
};

#endif // ORM2POSTGRES_H
