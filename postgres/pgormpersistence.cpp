#include "pgormpersistence.h"
#include "orm_implementions/t0035_blobs.h"

PGORMPersistence::PGORMPersistence(ORMSqlInterface &sqlInterface):ORMPersistenceInterface(sqlInterface) {}

ORMUuid PGORMPersistence::storeBlob(const std::basic_string<std::byte> &data, const reducedsole::uuid &created_by_t0003_id)
{
    t0035_blobs t0035;
    t0035.setid(ExtUuid::generateUuid());
    t0035.setblob_oid(ORMPersistenceInterface::storeBlob(data));
    insertObject(t0035, created_by_t0003_id);
    return t0035.getid();
}

bool PGORMPersistence::fetchBlob(const ORMUuid &blobUuid, std::basic_string<std::byte> &data)
{
    t0035_blobs t0035;
    if (!selectObject(blobUuid, t0035))
    {
        return false;
    }
    return ORMPersistenceInterface::fetchBlob(t0035.getblob_oid(), data);
}

bool PGORMPersistence::deleteBlob(const ORMUuid &blobUuid, const reducedsole::uuid &deleted_by_t0003_id)
{
    t0035_blobs t0035;
    if (!selectObject(blobUuid, t0035))
    {
        return false;
    }
    deleteObject(t0035, deleted_by_t0003_id);
    return true;

}
