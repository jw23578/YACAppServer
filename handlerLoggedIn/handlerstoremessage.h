#ifndef HANDLERSTOREMESSAGE_H
#define HANDLERSTOREMESSAGE_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "databaselogic/databaselogicmessages.h"

class HandlerStoreMessage : public HandlerLoggedInInterface
{
    DatabaseLogicMessages &databaseLogicMessages;
public:
    HandlerStoreMessage(DatabaseLogicMessages &databaseLogicMessages,
                        PistacheServerInterface &serverInterface,
                        LoggedInAppUsersContainer &loggedInAppUsersContainer);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERSTOREMESSAGE_H
