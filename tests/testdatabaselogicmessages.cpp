#include "testdatabaselogicmessages.h"
#include <iostream>

TestDatabaseLogicMessages::TestDatabaseLogicMessages(DatabaseLogicMessages &databaseLogicMessage)
{
    reducedsole::uuid message_id(reducedsole::uuid4());
    reducedsole::uuid sender_id(reducedsole::uuid4());
    reducedsole::uuid to_id(reducedsole::uuid4());
    std::string content_base64("nur ein test text");;
    databaseLogicMessage.storeMessage(message_id,
                                      sender_id,
                                      to_id,
                                      content_base64);

    rapidjson::Document document;
    rapidjson::Value messages;
    databaseLogicMessage.fetchMessages(to_id, std::chrono::system_clock::now() - std::chrono::minutes(10),
                                       messages,
                                       document.GetAllocator());

    if (messages.GetArray().Size() != 1)
    {
        return;
    }

    for (const auto &m : messages.GetArray())
    {
        std::cout << m["content_base64"].GetString()   << std::endl;
    }

    databaseLogicMessage.deleteMessage(message_id);

    databaseLogicMessage.setRead(message_id, message_id, std::chrono::system_clock::now());
    databaseLogicMessage.setRead(message_id, message_id, std::chrono::system_clock::now());
    databaseLogicMessage.setRead(message_id, message_id, std::chrono::system_clock::now());

    rapidjson::Value readMessages;
    databaseLogicMessage.fetchReadMessages(message_id,
                                          std::chrono::system_clock::now() - std::chrono::minutes(100),
                                          readMessages,
                                           document.GetAllocator());
    for (const auto &m : messages.GetArray())
    {
        std::cout << m["id"].GetString()   << std::endl;
    }
}
