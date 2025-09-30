#include "handlerappuserinsertworktime.h"

HandlerAppUserInsertWorktime::HandlerAppUserInsertWorktime(DatabaseLogics &databaseLogics,
                                                           PistacheServerInterface &serverInterface,
                                                           LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
                             methodNames.insertWorktime,
                             TypePost,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{
    addMethod(serverInterface,
              methodNames.insertWorktimeBeginEnd,
              TypePost);
    addMethod(serverInterface,
              methodNames.deleteWorktime,
              TypePost);
}

void HandlerAppUserInsertWorktime::method(CurrentContext &context)
{
    DatabaseLogicWorktime &dlwt(databaseLogics.databaseLogicWorktime);
    if (isMethod(methodNames.deleteWorktime))
    {
        MACRO_GetMandatoryUuid(id);
        std::string message;
        answerOk(message, dlwt.deleteWorktime(context.userId,
                                              id,
                                              message));
        return;
    }
    if (isMethod(methodNames.insertWorktimeBeginEnd))
    {
        MACRO_GetMandatoryTimePointFromISO(beginISO);
        MACRO_GetMandatoryTimePointFromISO(endISO);
        MACRO_GetMandatoryInt(worktimeType, false);
        std::string message;
        answerOk(message, dlwt.insertWorktimeBeginEnd(context.userId,
                                                      beginISO,
                                                      endISO,
                                                      static_cast<DatabaseLogicWorktime::WorktimeType>(worktimeType),
                                                      message));
        return;
    }
    MACRO_GetMandatoryTimePointFromISO(ts);
    MACRO_GetMandatoryInt(worktimeType, false);
    MACRO_GetMandatoryInt(userMood, true);
    MACRO_GetMandatoryInt(dayRating, true);
    std::string message;
    std::chrono::system_clock::time_point workStart;
    std::chrono::system_clock::time_point pauseStart;
    std::chrono::system_clock::time_point offSiteWorkStart;
    bool success(dlwt.insertWorktime(context.userId, ts,
                                     static_cast<DatabaseLogicWorktime::WorktimeType>(worktimeType),
                                     static_cast<DatabaseLogicWorktime::UserMood>(userMood),
                                     static_cast<DatabaseLogicWorktime::DayRating>(dayRating),
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
