#include "handlerappuserinsertworktime.h"

HandlerAppUserInsertWorktime::HandlerAppUserInsertWorktime(DatabaseLogics &databaseLogics,
                                                           PistacheServerInterface &serverInterface,
                                                           LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/insertWorktime",
                             TypePost,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{

}

void HandlerAppUserInsertWorktime::method()
{
    MACRO_GetMandatoryTimePointFromISO(ts);
    MACRO_GetMandatoryInt(worktimeType, false);
    std::string message;
    std::chrono::system_clock::time_point workStart;
    std::chrono::system_clock::time_point pauseStart;
    std::chrono::system_clock::time_point offSiteWorkStart;
    bool success(databaseLogics.databaseLogicWorktime.insertWorktime(userId, ts,
                                                                     static_cast<DatabaseLogicWorktime::WorktimeType>(worktimeType),
                                                                     workStart,
                                                                     pauseStart,
                                                                     offSiteWorkStart,
                                                                     message));
    std::map<std::string, std::string> data;
    data["workStart"] = ExtString::timepointToISO(workStart);
    data["pauseStart"] = ExtString::timepointToISO(pauseStart);
    data["offSiteWorkStart"] = ExtString::timepointToISO(offSiteWorkStart);
    answerOk(message,
             success,
             data);
}
