#ifndef HANDLERLOGGEDININTERFACE_H
#define HANDLERLOGGEDININTERFACE_H

#include <pistachehandlerinterface.h>
#include "loggedincontainer/loggedincontainerinterface.h"

class HandlerLoggedInInterface : public PistacheHandlerInterface
{
    LoggedInContainerInterface &loggedInContainer;
    std::string loginEMail;
    std::string loginToken;
    std::string third;
    std::string mandant;
protected:
    reducedsole::uuid appId;
    reducedsole::uuid loggedInUserId;
public:
    HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                             LoggedInContainerInterface &loggedInContainer);
    HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                             const std::string &methodName,
                             HandlerType type,
                             LoggedInContainerInterface &loggedInContainer);

    // PistacheHandlerInterface interface
public:
    bool checkLogin() override;
};

#endif // HANDLERLOGGEDININTERFACE_H
