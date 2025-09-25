#ifndef PGORMPERSISTENCE_H
#define PGORMPERSISTENCE_H

#include <ormpersistenceinterface.h>

class PGORMPersistence : public ORMPersistenceInterface
{
public:
    PGORMPersistence(ORMSqlInterface &sqlInterface);

    // ORMPersistenceInterface interface
public:
    ORMUuid storeBlob(const std::basic_string<std::byte> &data, const reducedsole::uuid &created_by_t0002_id);
    bool fetchBlob(const ORMUuid &blobUuid, std::basic_string<std::byte> &data);
    bool deleteBlob(const ORMUuid &blobUuid, const reducedsole::uuid &deleted_by_t0002_id);
};

#endif // PGORMPERSISTENCE_H
