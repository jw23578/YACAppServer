#ifndef HANDLERREGISTER_H
#define HANDLERREGISTER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogic.h"
#include "emaillogic.h"

class HandlerRegister : public PistacheHandlerInterface
{
    DatabaseLogic &databaseLogic;
    EMailLogic &emailLogic;
public:
    HandlerRegister(DatabaseLogic &databaseLogic,
                    EMailLogic &emailLogic,
                    PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERREGISTER_H
