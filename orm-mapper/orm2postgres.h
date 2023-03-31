#ifndef ORM2POSTGRES_H
#define ORM2POSTGRES_H

#include "pgconnectionpool.h"
#include "orm/ormobjectinterface.h"
#include "tablefields.h"
#include "pgexecutor.h"

class ORM2Postgres
{
    PGConnectionPool &pool;
    const TableFields tableFields;
public:
    ORM2Postgres(PGConnectionPool &pool);

    void insertOrUpdate(const ORMObjectInterface &object);

    bool select(const sole::uuid &id,
                ORMObjectInterface &object);

    size_t selectAll(const ORMObjectInterface &ghost,
                     std::set<ORMObjectInterface*> &target);
};

#endif // ORM2POSTGRES_H
