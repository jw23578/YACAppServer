#ifndef DATABASELOGICIMAGETABLE_H
#define DATABASELOGICIMAGETABLE_H

#include "postgres/pgconnectionpool.h"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"


class DatabaseLogicImageTable
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;
    TableFields tableFields;
    std::map<reducedsole::uuid, pqxx::oid> imageId2oid;
    bool lookUpOid(const reducedsole::uuid &imageId,
                   pqxx::oid &imageOid);
public:
    DatabaseLogicImageTable(LogStatController &logStatController,
                            PGConnectionPool &pool);

    bool storeImage(const std::basic_string<std::byte> &data,
                    std::string &message,
                    reducedsole::uuid &imageId);
    bool fetchImage(const reducedsole::uuid &imageId,
                    std::string &message,
                    std::basic_string<std::byte> &data);
};

#endif // DATABASELOGICIMAGETABLE_H
