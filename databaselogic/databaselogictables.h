#ifndef DATABASELOGICTABLES_H
#define DATABASELOGICTABLES_H

#include "postgres/pgconnectionpool.h"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "orm_implementions/yacormfactory.h"


class DatabaseLogicTables
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    YACORMFactory &factory;
    PGUtils utils;
    TableNames tableNames;
    TableFields tableFields;
public:
    DatabaseLogicTables(LogStatController &logStatController,
                        PGConnectionPool &pool,
                        YACORMFactory &factory);

    bool connectionOk();
    bool pgCryptoInstalled();
    void createDatabaseTables();

};

#endif // DATABASELOGICTABLES_H
