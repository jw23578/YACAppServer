#ifndef HANDLERTEX2PDF_H
#define HANDLERTEX2PDF_H

#include "handlerloggedininterface.h"
#include "loggedincontainer/loggedinappuserscontainer.h"

class HandlerTex2Pdf : public HandlerLoggedInInterface
{
public:
    HandlerTex2Pdf(ORMPersistenceInterface &opi,
                   PistacheServerInterface &serverInterface,
                   LoggedInAppUsersContainer &loggedInAppUsersContainer);

public:
    void method(CurrentContext &context);
};

#endif // HANDLERTEX2PDF_H
