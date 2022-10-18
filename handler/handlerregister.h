#ifndef HANDLERREGISTER_H
#define HANDLERREGISTER_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogic.h"

class HandlerRegister : public PistacheHandlerInterface
{
    DatabaseLogic &databaseLogic;
public:
    HandlerRegister(DatabaseLogic &databaseLogic, PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERREGISTER_H
