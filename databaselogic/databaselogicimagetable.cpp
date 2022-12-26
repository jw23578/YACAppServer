#include "databaselogicimagetable.h"
#include "pgoidstorer.h"
#include "pgoidloader.h"
#include "pgexecutor.h"

DatabaseLogicImageTable::DatabaseLogicImageTable(LogStatController &logStatController,
                                                 PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    utils(pool)
{

}

bool DatabaseLogicImageTable::storeImage(const std::string &data,
                                         std::string &message,
                                         sole::uuid &imageId)
{
    pqxx::oid result;
    PGOidStorer storer(pool,
                       data,
                       result);
    PGSqlString sql;
    sql.insert(tableNames.t0013_images);
    imageId = sole::uuid4();
    sql.addInsert(tableFields.id, imageId);
    sql.addInsert(tableFields.data, data);
    PGExecutor e(pool,
                 sql);
    if (!e.size())
    {
        message = "could not store image";
        return false;
    }
    return true;
}
