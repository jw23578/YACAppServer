#ifndef ORM2POSTGRES_H
#define ORM2POSTGRES_H

#include "pgconnectionpool.h"
#include "orm_implementions/yacbaseobject.h"
#include "tablefields.h"
#include "pgexecutor.h"

class ORM2Postgres
{
    PGConnectionPool &pool;
    const TableFields tableFields;
public:
    ORM2Postgres(PGConnectionPool &pool);

    void insertOrUpdate(YACBaseObject &object);

    bool select(const sole::uuid &id,
                YACBaseObject &object);

    size_t selectAll(const YACBaseObject &ghost,
                     std::set<YACBaseObject*> &target);
};

#endif // ORM2POSTGRES_H
