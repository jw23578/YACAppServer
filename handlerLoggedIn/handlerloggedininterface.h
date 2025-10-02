#ifndef HANDLERLOGGEDININTERFACE_H
#define HANDLERLOGGEDININTERFACE_H

#include "interfaces/pistachehandlerinterface.h"
#include "loggedincontainer/loggedincontainerinterface.h"

class HandlerLoggedInInterface : public PistacheHandlerInterface
{
    LoggedInContainerInterface &loggedInContainer;
    std::string loginEMail;
    std::string loginToken;
    std::string third;
    std::string mandant;

public:
    HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                             ORMPersistenceInterface &opi,
                             LoggedInContainerInterface &loggedInContainer);
    HandlerLoggedInInterface(PistacheServerInterface &serverInterface,
                             ORMPersistenceInterface &opi,
                             const std::string &methodName,
                             HandlerType type,
                             LoggedInContainerInterface &loggedInContainer);

    // PistacheHandlerInterface interface
public:
    bool checkLogin(CurrentContext &context) override;
    bool logout(CurrentContext &context);
};

#endif // HANDLERLOGGEDININTERFACE_H
