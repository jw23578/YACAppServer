#ifndef DATABASELOGICIMAGETABLE_H
#define DATABASELOGICIMAGETABLE_H

#include "postgres/pgconnectionpool.h"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "tablefields.h"


class DatabaseLogicImageTable
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;
    TableFields tableFields;
public:
    DatabaseLogicImageTable(LogStatController &logStatController,
                            PGConnectionPool &pool);

    bool storeImage(const std::string &data,
                    std::string &message,
                    sole::uuid &imageId);
};

#endif // DATABASELOGICIMAGETABLE_H
