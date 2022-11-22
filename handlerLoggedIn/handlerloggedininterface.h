#ifndef HANDLERLOGGEDININTERFACE_H
#define HANDLERLOGGEDININTERFACE_H

#include <pistachehandlerinterface.h>
#include "loggedincontainer/loggedinuserscontainer.h"

class HandlerLoggedInInterface : public PistacheHandlerInterface
{
    LoggedInUsersContainer &loggedInUsersContainer;
    std::string loginEMail;
    std::string loginToken;
protected:
    sole::uuid userId;
public:
    HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                             const std::string &methodName,
                             HandlerType type,
                             LoggedInUsersContainer &loggedInUsersContainer);

    // PistacheHandlerInterface interface
public:
    bool checkLogin() override;
};

#endif // HANDLERLOGGEDININTERFACE_H
