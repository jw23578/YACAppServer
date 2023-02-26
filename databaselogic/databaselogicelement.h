#ifndef DATABASELOGICELEMENT_H
#define DATABASELOGICELEMENT_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"

class DatabaseLogicElement
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    TableNames tableNames;
    TableFields tableFields;
public:
    DatabaseLogicElement(LogStatController &logStatController,
                         PGConnectionPool &pool);

    bool insertElementUsable4AppUser(sole::uuid const &id,
                                     sole::uuid const &element_id,
                                     sole::uuid const &appuser_id);

    bool updateElementUsable4AppUser(sole::uuid const &id,
                                     sole::uuid const &element_id,
                                     sole::uuid const &appuser_id);

    bool deleteElementUsable4AppUser(sole::uuid const &id);

    bool insertElementVisible4AppUser(sole::uuid const &id,
                                      sole::uuid const &element_id,
                                      sole::uuid const &appuser_id);

    bool updateElementVisible4AppUser(sole::uuid const &id,
                                      sole::uuid const &element_id,
                                      sole::uuid const &appuser_id);

    bool deleteElementVisible4AppUser(sole::uuid const &id);
};

#endif // DATABASELOGICELEMENT_H
