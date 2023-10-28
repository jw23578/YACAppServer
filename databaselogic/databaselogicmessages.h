#ifndef DATABASELOGICMESSAGES_H
#define DATABASELOGICMESSAGES_H

#include "databaselogicinterface.h"
#include "sole/sole.hpp"
#include "rapidjson/document.h"
#include "orm_implementions/t0028_message_images.h"

class DatabaseLogicMessages: public DatabaseLogicInterface
{
    std::map<sole::uuid, pqxx::oid> imageId2oid;
    bool lookUpOid(const sole::uuid &imageId,
                   pqxx::oid &imageOid);
public:
    //    struct Message
    //    {
    //        sole::uuid id;
    //        sole::uuid sender_id;
    //        std::chrono::system_clock::time_point sended_datetime;
    //        std::string content_base64;
    //    };

    DatabaseLogicMessages(LogStatController &logStatController,
                          PGConnectionPool &pool);

    void storeMessage(const sole::uuid &id,
                      const sole::uuid &sender_id,
                      const sole::uuid &to_id,
                      const std::string &content_base64);

    void deleteMessage(const sole::uuid &id);

    bool markMessageDeleted(const sole::uuid &id,
                            const sole::uuid &sender_id,
                            std::string &resultMessage);

    bool fetchMessages(const sole::uuid &fetcher_id,
                       const std::chrono::system_clock::time_point &since,
                       rapidjson::Value &target,
                       rapidjson::MemoryPoolAllocator<> &alloc);
    bool fetchReceivedMessages(const sole::uuid &receiver_id,
                               const std::chrono::system_clock::time_point &since,
                               rapidjson::Value &target,
                               rapidjson::MemoryPoolAllocator<> &alloc);
    bool fetchReadMessages(const sole::uuid &reader_id,
                           const std::chrono::system_clock::time_point &since,
                           rapidjson::Value &target,
                           rapidjson::MemoryPoolAllocator<> &alloc);
    bool fetchReceivedAndReadMessages(const sole::uuid &receiver_or_reader_id,
                                      const std::chrono::system_clock::time_point &since,
                                      rapidjson::Value &targetReceived,
                                      rapidjson::Value &targetRead,
                                      rapidjson::MemoryPoolAllocator<> &alloc);

    void setReceived(const sole::uuid &receiver_id,
                     const sole::uuid &message_id,
                     const std::chrono::system_clock::time_point &received_datetime);

    void setRead(const sole::uuid &reader_id,
                 const sole::uuid &message_id,
                 const std::chrono::system_clock::time_point &read_datetime);

    bool storeImage(t0028_message_images &t0028);
    bool fetchImage(const sole::uuid &imageId,
                                           std::string &message,
                                           std::basic_string<std::byte> &data);
};

#endif // DATABASELOGICMESSAGES_H
