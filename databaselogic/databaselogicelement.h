#ifndef DATABASELOGICELEMENT_H
#define DATABASELOGICELEMENT_H

#include "postgres/pgconnectionpool.h"
#include "JWUtils/reducedsole.h"
#include "logstatcontroller.h"
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

    bool insertElementUsable4AppUser(reducedsole::uuid const &id,
                                     reducedsole::uuid const &element_id,
                                     reducedsole::uuid const &appuser_id);

    bool updateElementUsable4AppUser(reducedsole::uuid const &id,
                                     reducedsole::uuid const &element_id,
                                     reducedsole::uuid const &appuser_id);

    bool deleteElementUsable4AppUser(reducedsole::uuid const &id);

    bool insertElementVisible4AppUser(reducedsole::uuid const &id,
                                      reducedsole::uuid const &element_id,
                                      reducedsole::uuid const &appuser_id);

    bool updateElementVisible4AppUser(reducedsole::uuid const &id,
                                      reducedsole::uuid const &element_id,
                                      reducedsole::uuid const &appuser_id);

    bool deleteElementVisible4AppUser(reducedsole::uuid const &id);
};

#endif // DATABASELOGICELEMENT_H
