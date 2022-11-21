#ifndef DATABASELOGICMESSAGES_H
#define DATABASELOGICMESSAGES_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"

class DatabaseLogicMessages
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;

    struct Message
    {
        sole::uuid id;
        sole::uuid sender_id;
        std::chrono::system_clock::time_point sended_datetime;
        std::string content_base64;
    };

public:
    DatabaseLogicMessages(LogStatController &logStatController,
                          PGConnectionPool &pool);

    void storeMessage(const sole::uuid &id,
                      const sole::uuid &sender_id,
                      const sole::uuid &to_id,
                      const std::string &content_base64);

    void deleteMessage(const sole::uuid &id);

    void fetchMessages(const sole::uuid &fetcher_id,
                       const std::chrono::system_clock::time_point &since,
                       std::vector<Message> &messages);
    void setReceived(const sole::uuid &receiver_id,
                     const std::set<sole::uuid> &message_ids);
};

#endif // DATABASELOGICMESSAGES_H
