#ifndef HANDLERUSERREGISTER_H
#define HANDLERUSERREGISTER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogicuserandapp.h"
#include "emaillogic.h"

class HandlerUserRegister : public PistacheHandlerInterface
{
    DatabaseLogicUserAndApp &databaseLogicUserAndApp;
    EMailLogic &emailLogic;
public:
    HandlerUserRegister(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                        EMailLogic &emailLogic,
                        PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERUSERREGISTER_H
