#ifndef HANDLERXRECHNUNG_H
#define HANDLERXRECHNUNG_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "postgres/pgormpersistence.h"

class HandlerXRechnung : public HandlerLoggedInInterface
{
public:
    HandlerXRechnung(PistacheServerInterface &serverInterface,
                     LoggedInAppUsersContainer &loggedInAppUsersContainer,
                     PGORMPersistence &opi);

    // PistacheHandlerInterface interface
public:
    void method(CurrentContext &context);
};

#endif // HANDLERXRECHNUNG_H
