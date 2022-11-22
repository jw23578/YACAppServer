#ifndef LOGGEDINCONTAINERINTERFACE_H
#define LOGGEDINCONTAINERINTERFACE_H

#include "sole/sole.hpp"

class LoggedInContainerInterface
{
public:
    LoggedInContainerInterface();

    virtual bool isLoggedIn(const std::string &loginEMail,
                            const std::string &loginToken,
                            sole::uuid &userId) = 0;

    virtual bool isLoggedIn(const std::string &loginEMail,
                            const std::string &loginToken) = 0;
};

#endif // LOGGEDINCONTAINERINTERFACE_H
