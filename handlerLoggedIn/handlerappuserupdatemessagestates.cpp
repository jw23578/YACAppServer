#include "handlerappuserupdatemessagestates.h"

HandlerAppUserUpdateMessageStates::HandlerAppUserUpdateMessageStates(PistacheServerInterface &serverInterface,
                                                                     DatabaseLogics &databaseLogics,
                                                                     LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
                             "/updateMessageStates",
                             TypePost,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{

}

void HandlerAppUserUpdateMessageStates::method(CurrentContext &context)
{
    const rapidjson::Value &messages(getPostedJsonValue("messages"));
    if (!messages.IsArray())
    {
       answerBad("missing messages");
       return;
    }
    for (const auto &m: messages.GetArray())
    {
        reducedsole::uuid messageId(reducedsole::rebuild(m["id"].GetString()));
        if (m.HasMember("readAtISO"))
        {
            std::chrono::system_clock::time_point readAt(ExtString::toTimepoint(m["readAtIDO"].GetString()));
            databaseLogics.databaseLogicMessages.setRead(context.userId,
                                                         messageId,
                                                         readAt);
        }
    }
}
