#ifndef HANDLERLOGGEDININTERFACE_H
#define HANDLERLOGGEDININTERFACE_H

#include <pistachehandlerinterface.h>
#include "loggedincontainer/loggedincontainerinterface.h".h"

class HandlerLoggedInInterface : public PistacheHandlerInterface
{
    LoggedInContainerInterface &loggedInContainer;
    std::string loginEMail;
    std::string loginToken;
protected:
    sole::uuid userId;
public:
    HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                             const std::string &methodName,
                             HandlerType type,
                             LoggedInContainerInterface &loggedInContainer);

    // PistacheHandlerInterface interface
public:
    bool checkLogin() override;
};

#endif // HANDLERLOGGEDININTERFACE_H
