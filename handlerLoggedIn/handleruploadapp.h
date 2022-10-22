#ifndef HANDLERUPLOADAPP_H
#define HANDLERUPLOADAPP_H

#include "handlerloggedininterface.h"

class HandlerUploadApp : public HandlerLoggedInInterface
{
public:
    HandlerUploadApp(PistacheServerInterface &serverInterface,
                     LoggedInUsersContainer &loggedInUsersContainer);

    // PistacheHandlerInterface interface
public:
    void method() override;
};

#endif // HANDLERUPLOADAPP_H
