#ifndef HANDLERGETALLAPPS_H
#define HANDLERGETALLAPPS_H

#include <pistachehandlerinterface.h>
#include "databaselogic.h"

class HandlerGetAllApps : public PistacheHandlerInterface
{
    DatabaseLogic &databaseLogic;
public:
    HandlerGetAllApps(DatabaseLogic &databaseLogic,
                      PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERGETALLAPPS_H
