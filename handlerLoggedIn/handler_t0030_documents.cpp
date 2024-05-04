#include "handler_t0030_documents.h"
#include "orm_implementions/t0030_documents.h"

Handler_t0030_documents::Handler_t0030_documents(PistacheServerInterface &serverInterface,
                                                 LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface, loggedInAppUsersContainer)
{
    addAllMethodTypes(serverInterface, t0030_documents().getORMName());
}
