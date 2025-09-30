#include "handlerappuserfetchmessageupdates.h"

HandlerAppUserFetchMessageUpdates::HandlerAppUserFetchMessageUpdates(PistacheServerInterface &serverInterface,
                                                                     DatabaseLogics &databaseLogics,
                                                                     LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
                             "/fetchMessageUpdates",
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{

}

void HandlerAppUserFetchMessageUpdates::method(CurrentContext &context)
{
    MACRO_GetMandatoryTimePointFromISO(updatesSinceISO);
    rapidjson::Document document;
    document.SetObject();
    std::string serverNowISO(ExtString::timepointToISO(std::chrono::system_clock::now()));
    document.AddMember("serverNowISO", serverNowISO, document.GetAllocator());
    rapidjson::Value messages;
    DatabaseLogicMessages &dlm(databaseLogics.databaseLogicMessages);
    if (!dlm.fetchMessages(context.userId,
                           updatesSinceISO,
                           messages,
                           document.GetAllocator()))
    {
        answerOk("error on fetching messages", false);
        return;
    }
    document.AddMember("messages", messages, document.GetAllocator());
    rapidjson::Value receivedMessages;
    rapidjson::Value readMessages;
    if (!dlm.fetchReceivedAndReadMessages(context.userId,
                                          updatesSinceISO,
                                          receivedMessages,
                                          readMessages,
                                          document.GetAllocator()))
    {
        answerOk("error on fetching received and read messages", false);
        return;
    }
    document.AddMember("readMessages", readMessages, document.GetAllocator());
    document.AddMember("receivedMessages", receivedMessages, document.GetAllocator());

    DatabaseLogicSpaces &dls(databaseLogics.databaseLogicSpaces);
    rapidjson::Value spaceRequests;
    std::string errorMessage;
    if (!dls.fetchSpaceRequests(context.userId,
                                spaceRequests,
                                document.GetAllocator(),
                                errorMessage))
    {
        answerOk(errorMessage, false);
        return;
    }
    document.AddMember("spaceRequests", spaceRequests, document.GetAllocator());
    answerOk(true, document);
}
