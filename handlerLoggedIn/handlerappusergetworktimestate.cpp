#include "handlerappusergetworktimestate.h"

HandlerAppUserGetWorktimeState::HandlerAppUserGetWorktimeState(DatabaseLogics &databaseLogics,
                                                               PistacheServerInterface &serverInterface,
                                                               LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
                             methodNames.getWorktimeState,
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{
    addMethod(serverInterface,
              methodNames.fetchWorktimes,
              TypeGet);
}

void HandlerAppUserGetWorktimeState::method(CurrentContext &context)
{
    if (isMethod(methodNames.fetchWorktimes))
    {
        MACRO_GetMandatoryTimePointFromISO(sinceISO);
        MACRO_GetMandatoryTimePointFromISO(untilISO);
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value worktimes;
        std::string message;
        if (!databaseLogics.databaseLogicWorktime.fetchWorktimes(context,
                                                                 context.userId,
                                                                 sinceISO,
                                                                 untilISO,
                                                                 worktimes,
                                                                 document.GetAllocator(),
                                                                 message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("worktimes", worktimes, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    std::chrono::system_clock::time_point workStart;
    std::chrono::system_clock::time_point pauseStart;
    std::chrono::system_clock::time_point offSiteWorkStart;
    if (!databaseLogics.databaseLogicWorktime.currentState(context,
                                                           context.userId,
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
