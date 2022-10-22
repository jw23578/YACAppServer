#include "handleruploadapp.h"

HandlerUploadApp::HandlerUploadApp(PistacheServerInterface &serverInterface,
                                   LoggedInUsersContainer &loggedInUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/uploadApp",
                             TypePost,
                             loggedInUsersContainer)
{

}

void HandlerUploadApp::method()
{

}
