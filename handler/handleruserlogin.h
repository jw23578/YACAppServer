#ifndef HANDLERUSERLOGIN_H
#define HANDLERUSERLOGIN_H

#include "interfaces/pistachehandlerinterface.h"
#include "databaselogicuserandapp.h"

class HandlerUserLogin : public PistacheHandlerInterface
{
    DatabaseLogicUserAndApp &databaseLogicUserAndApp;
public:
    HandlerUserLogin(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                     PistacheServerInterface &serverInterface);

    // PistacheHandlerInterface interface
public:
    void method();
};

#endif // HANDLERUSERLOGIN_H
