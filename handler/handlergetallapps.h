#ifndef HANDLERGETALLAPPS_H
#define HANDLERGETALLAPPS_H

#include <pistachehandlerinterface.h>
#include "databaselogicuserandapp.h"

class HandlerGetAllApps : public PistacheHandlerInterface
{
    DatabaseLogicUserAndApp &databaseLogicUserAndApp;
public:
    HandlerGetAllApps(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                      PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERGETALLAPPS_H
