#include "handlerappuserfetchmessageupdates.h"

HandlerAppUserFetchMessageUpdates::HandlerAppUserFetchMessageUpdates(PistacheServerInterface &serverInterface,
                                                                     DatabaseLogics &databaseLogics,
                                                                     LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/fetchMessageUpdates",
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{

}

void HandlerAppUserFetchMessageUpdates::method()
{
    MACRO_GetMandatoryTimePointFromISO(updatesSinceISO);

    rapidjson::Document document;
    document.AddMember("serverNowISO", ExtString::timepointToISO(std::chrono::system_clock::now()), document.GetAllocator());
    rapidjson::Value messages;
    if (!databaseLogics.databaseLogicMessages.fetchMessages(userId,
                                                            updatesSinceISO,
                                                            messages,
                                                            document.GetAllocator()))
    {
        answerOk("error on fetching messages", false);
        return;
    }
    document.AddMember("messsages", messages, document.GetAllocator());
    rapidjson::Value readMessages;
    if (!databaseLogics.databaseLogicMessages.fetchReadMessages(userId,
                                                                updatesSinceISO,
                                                                readMessages,
                                                                document.GetAllocator()))
    {
        answerOk("error on fetching read messages", false);
        return;
    }
    document.AddMember("readMessages", readMessages, document.GetAllocator());
    rapidjson::Value receivedMessages;
    if (!databaseLogics.databaseLogicMessages.fetchReceivedMessages(userId,
                                                                    updatesSinceISO,
                                                                    receivedMessages,
                                                                    document.GetAllocator()))
    {
        answerOk("error on fetching received messages", false);
        return;
    }
    document.AddMember("receivedMessages", receivedMessages, document.GetAllocator());
    answerOk(true, document);
}
