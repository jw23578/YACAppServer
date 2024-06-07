#include "databaselogicimagetable.h"
#include "pgoidstorer.h"
#include "pgoidloader.h"
#include "pgexecutor.h"

bool DatabaseLogicImageTable::lookUpOid(const sole::uuid &imageId,
                                        pqxx::oid &imageOid)
{
    PGExecutor e(pool);
    e.select(tableNames.t0013_images,
             tableFields.id,
             imageId);
    if (!e.size())
    {
        return false;
    }
    imageOid = e.oid(tableFields.data);
    return true;
}

DatabaseLogicImageTable::DatabaseLogicImageTable(LogStatController &logStatController,
                                                 PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    utils(pool)
{

}

bool DatabaseLogicImageTable::storeImage(const std::basic_string<std::byte> &data,
                                         std::string &message,
                                         sole::uuid &imageId)
{
    pqxx::oid imageOid;
    PGOidStorer storer(pool,
                       data,
                       imageOid);
    SqlString sql;
    sql.insert(tableNames.t0013_images);
    imageId = sole::uuid4();
    sql.addInsert(tableFields.id, imageId);
    sql.addInsert(tableFields.data, imageOid);
    PGExecutor e(pool,
                 sql);
    imageId2oid[imageId] = imageOid;
    return true;
}

bool DatabaseLogicImageTable::fetchImage(const sole::uuid &imageId,
                                         std::string &message,
                                         std::basic_string<std::byte> &data)
{
    auto it(imageId2oid.find(imageId));
    pqxx::oid imageOid(0);
    if (it == imageId2oid.end())
    {
        if (!lookUpOid(imageId, imageOid))
        {
            message = "could not look up image oid for imageid: " + ExtString::toString(imageId);
            return false;
        }
    }
    else
    {
        imageOid = it->second;
    }

    PGOidLoader loader(pool, imageOid, data);
    if (data.size() == 0)
    {
        message = "could not load image with oid: " + ExtString::toString(imageOid);
        return false;
    }
    return true;
}
