#ifndef DATABASELOGICMESSAGES_H
#define DATABASELOGICMESSAGES_H

#include "databaselogicinterface.h"
#include "JWUtils/reducedsole.h"
#include "rapidjson/document.h"
#include "orm_implementions/t0028_message_images.h"

class DatabaseLogicMessages: public DatabaseLogicInterface
{
    std::map<reducedsole::uuid, pqxx::oid> imageId2oid;
    bool lookUpOid(const reducedsole::uuid &imageId,
                   pqxx::oid &imageOid);
public:
    //    struct Message
    //    {
    //        reducedsole::uuid id;
    //        reducedsole::uuid sender_id;
    //        std::chrono::system_clock::time_point sended_datetime;
    //        std::string content_base64;
    //    };

    DatabaseLogicMessages(LogStatController &logStatController,
                          PGConnectionPool &pool);

    void storeMessage(const reducedsole::uuid &id,
                      const reducedsole::uuid &sender_id,
                      const reducedsole::uuid &to_id,
                      const std::string &content_base64);

    void deleteMessage(const reducedsole::uuid &id);

    bool markAllOfUserMessageDeleted(const reducedsole::uuid &user_id,
                                     std::string &resultMessage);

    bool markMessageDeleted(const reducedsole::uuid &id,
                            const reducedsole::uuid &sender_id,
                            std::string &resultMessage);

    bool fetchMessages(const reducedsole::uuid &fetcher_id,
                       const std::chrono::system_clock::time_point &since,
                       rapidjson::Value &target,
                       rapidjson::MemoryPoolAllocator<> &alloc);
    bool fetchReceivedMessages(const reducedsole::uuid &receiver_id,
                               const std::chrono::system_clock::time_point &since,
                               rapidjson::Value &target,
                               rapidjson::MemoryPoolAllocator<> &alloc);
    bool fetchReadMessages(const reducedsole::uuid &reader_id,
                           const std::chrono::system_clock::time_point &since,
                           rapidjson::Value &target,
                           rapidjson::MemoryPoolAllocator<> &alloc);
    bool fetchReceivedAndReadMessages(const reducedsole::uuid &receiver_or_reader_id,
                                      const std::chrono::system_clock::time_point &since,
                                      rapidjson::Value &targetReceived,
                                      rapidjson::Value &targetRead,
                                      rapidjson::MemoryPoolAllocator<> &alloc);

    void setReceived(const reducedsole::uuid &receiver_id,
                     const reducedsole::uuid &message_id,
                     const std::chrono::system_clock::time_point &received_datetime);

    void setRead(const reducedsole::uuid &reader_id,
                 const reducedsole::uuid &message_id,
                 const std::chrono::system_clock::time_point &read_datetime);

    bool storeImage(t0028_message_images &t0028);
    bool fetchImage(const reducedsole::uuid &imageId,
                    std::string &message,
                    std::basic_string<std::byte> &data);
};

#endif // DATABASELOGICMESSAGES_H
