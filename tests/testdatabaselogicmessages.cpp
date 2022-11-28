#include "testdatabaselogicmessages.h"
#include <iostream>

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

    std::vector<DatabaseLogicMessages::Message> messages;
    databaseLogicMessage.fetchMessages(to_id, std::chrono::system_clock::now() - std::chrono::minutes(10),
                                       messages);

    if (messages.size() != 1)
    {
        exit(1);
    }

    for (const auto &m : messages)
    {
        std::cout << m.content_base64 << std::endl;
    }

    databaseLogicMessage.deleteMessage(message_id);
}