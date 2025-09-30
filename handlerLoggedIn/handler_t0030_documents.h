#ifndef HANDLER_T0030_DOCUMENTS_H
#define HANDLER_T0030_DOCUMENTS_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"
#include "postgres/pgormpersistence.h"

class Handler_t0030_documents : public HandlerLoggedInInterface
{
public:
    Handler_t0030_documents(PistacheServerInterface &serverInterface,
                            LoggedInAppUsersContainer &loggedInAppUsersContainer,
                            PGORMPersistence &opi);



    // PistacheHandlerInterface interface
public:
    void method(CurrentContext &context) override;
};

#endif // HANDLER_T0030_DOCUMENTS_H
