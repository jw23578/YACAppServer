#ifndef DATABASELOGICTABLES_H
#define DATABASELOGICTABLES_H

#include "postgres/pgconnectionpool.h"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "tablefields.h"


class DatabaseLogicTables
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;
    TableFields tableFields;
public:
    DatabaseLogicTables(LogStatController &logStatController,
                        PGConnectionPool &pool);

    bool connectionOk();
    bool pgCryptoInstalled();
    void createDatabaseTables();

};

#endif // DATABASELOGICTABLES_H
