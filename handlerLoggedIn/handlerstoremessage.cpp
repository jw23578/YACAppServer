#include "handlerstoremessage.h"

HandlerStoreMessage::HandlerStoreMessage(DatabaseLogicMessages &databaseLogicMessages,
                                         PistacheServerInterface &serverInterface,
                                         LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/storeMessage",
                             TypePost,
                             loggedInAppUsersContainer),
    databaseLogicMessages(databaseLogicMessages)
{

}

void HandlerStoreMessage::method()
{
    MACRO_GetMandatoryUuid(id);
    MACRO_GetMandatoryUuid(to_id);
    MACRO_GetMandatoryString(content_base64);

    databaseLogicMessages.storeMessage(id,
                                       userId,
                                       to_id,
                                       content_base64);

    answerOk("message stored", true);
}
