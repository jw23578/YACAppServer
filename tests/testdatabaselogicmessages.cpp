#include "testdatabaselogicmessages.h"

TestDatabaseLogicMessages::TestDatabaseLogicMessages(DatabaseLogicMessages &databaseLogicMessage)
{
    sole::uuid message_id(sole::uuid4());
    sole::uuid sender_id(sole::uuid4());
    sole::uuid to_id(sole::uuid4());
    std::string content_base64("nur ein test text");;
    databaseLogicMessage.storeMessage(message_id,
                                      sender_id,
                                      to_id,
                                      content_base64);

    databaseLogicMessage.deleteMessage(message_id);
}
