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
    answerOk(message, databaseLogics.databaseLogicWorktime.insertWorktime(userId, ts,
                                                                          static_cast<DatabaseLogicWorktime::WorktimeType>(worktimeType), message));
}
