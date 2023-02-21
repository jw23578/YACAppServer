#ifndef DATABASELOGICMESSAGES_H
#define DATABASELOGICMESSAGES_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"

class DatabaseLogicMessages
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;
    TableFields tableFields;

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

    void setReceived(const sole::uuid &receiver_id,
                     const sole::uuid &message_id,
                     const std::chrono::system_clock::time_point &received_datetime);

    void setRead(const sole::uuid &reader_id,
                 const sole::uuid &message_id,
                 const std::chrono::system_clock::time_point &read_datetime);
};

#endif // DATABASELOGICMESSAGES_H
