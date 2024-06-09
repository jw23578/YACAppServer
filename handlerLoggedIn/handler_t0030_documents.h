#ifndef HANDLER_T0030_DOCUMENTS_H
#define HANDLER_T0030_DOCUMENTS_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "orm/ormpersistenceinterface.h"

class Handler_t0030_documents : public HandlerLoggedInInterface
{
    ORMPersistenceInterface &opi;
public:
    Handler_t0030_documents(PistacheServerInterface &serverInterface,
                            LoggedInAppUsersContainer &loggedInAppUsersContainer,
                            ORMPersistenceInterface &opi);



    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLER_T0030_DOCUMENTS_H
