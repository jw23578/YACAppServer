#include "handleruploadapp.h"

HandlerUploadApp::HandlerUploadApp(DatabaseLogic &databaseLogic,
                                   PistacheServerInterface &serverInterface,
                                   LoggedInUsersContainer &loggedInUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/uploadApp",
                             TypePost,
                             loggedInUsersContainer),
    databaseLogic(databaseLogic)
{

}

void HandlerUploadApp::method()
{
    MACRO_GetMandatoryPostString(appId);
    MACRO_GetMandatoryPostString(json_yacapp);
    MACRO_GetMandatoryPostString(yacpck_base64);

    databaseLogic.saveApp(appId,
                          json_yacapp,
                          yacpck_base64);

    answer(Pistache::Http::Code::Ok, "new yacApp stored");
}
