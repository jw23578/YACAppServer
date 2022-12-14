#include "handlerappusergetworktimestate.h"

HandlerAppUserGetWorktimeState::HandlerAppUserGetWorktimeState(DatabaseLogics &databaseLogics,
                                                               PistacheServerInterface &serverInterface,
                                                               LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/getWorktimeState",
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{

}

void HandlerAppUserGetWorktimeState::method()
{
    std::chrono::system_clock::time_point workStart;
    std::chrono::system_clock::time_point pauseStart;
    std::chrono::system_clock::time_point offSiteWorkStart;
    if (!databaseLogics.databaseLogicWorktime.currentState(userId,
                                                           workStart,
                                                           pauseStart,
                                                           offSiteWorkStart))
    {
        answerOk("", false);
        return;
    }
    std::map<std::string, std::string> data;
    data["workStart"] = ExtString::timepointToISO(workStart);
    data["pauseStart"] = ExtString::timepointToISO(pauseStart);
    data["offSiteWorkStart"] = ExtString::timepointToISO(offSiteWorkStart);
    answerOk("current work state",
             true,
             data);
}
