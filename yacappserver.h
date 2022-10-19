#ifndef YACAPPSERVER_H
#define YACAPPSERVER_H

#include "interfaces/pistacheserverinterface.h"
#include "handler/handlerregister.h"
#include "databaselogic.h"
#include "emaillogic.h"

class YACAppServer: public PistacheServerInterface
{
    DatabaseLogic &databaseLogic;
    EMailLogic &emailLogic;
    HandlerRegister handlerRegister;

public:
    YACAppServer(DatabaseLogic &databaseLogic,
                 EMailLogic &emailLogic,
                 int port);
};

#endif // YACAPPSERVER_H
