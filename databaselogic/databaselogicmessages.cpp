#include "databaselogicmessages.h"
#include "pgexecutor.h"
#include "definitions.h"

DatabaseLogicMessages::DatabaseLogicMessages(LogStatController &logStatController,
                                             PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    utils(pool)
{

}

void DatabaseLogicMessages::storeMessage(const sole::uuid &id,
                                         const sole::uuid &sender_id,
                                         const sole::uuid &to_id,
                                         const std::string &content_base64)
{
    PGSqlString sql(utils.createInsertString(tableNames.t0007_messages));
    MACRO_set(id);
    MACRO_set(sender_id);
    MACRO_set(to_id);
    MACRO_set(content_base64);
    sql.set("sended_datetime", TimePointPostgreSqlNow);
    PGExecutor e(pool, sql);
}

void DatabaseLogicMessages::deleteMessage(const sole::uuid &id)
{
    PGSqlString sql("delete from ");
    sql += tableNames.t0007_messages;
    sql += " where id = :id";
    MACRO_set(id);
    PGExecutor e(pool, sql);
}

bool DatabaseLogicMessages::fetchMessages(const sole::uuid &fetcher_id,
                                          const std::chrono::system_clock::time_point &since,
                                          rapidjson::Value &target,
                                          rapidjson::MemoryPoolAllocator<> &alloc)
{
    PGSqlString sql("select * from ");
    sql += tableNames.t0007_messages;
    sql += " where (sender_id = :fetcher_id or to_id = :fetcher_id ";
    sql += " or to_id in (select group_id from  ";
    sql += tableNames.t0006_appuser2group;
    sql += " where appuser_id = :fetcher_id)) ";
    sql += " and sended_datetime >= :since";
    MACRO_set(fetcher_id);
    MACRO_set(since);
    PGExecutor e(pool, sql);
    e.toJsonArray(target, alloc);
//    for (size_t i(0); i < e.size(); ++i)
//    {
//        Message m;
//        m.id = e.uuid("id");
//        m.sender_id = e.uuid("sender_id");
//        m.sended_datetime = e.timepoint("sended_datetime");
//        m.content_base64 = e.string("content_base64");
//        messages.push_back(m);
//        e.next();
//    }
    return true;
}

bool DatabaseLogicMessages::fetchReceivedMessages(const sole::uuid &receiver_id,
                                                  const std::chrono::system_clock::time_point &since,
                                                  rapidjson::Value &target,
                                                  rapidjson::MemoryPoolAllocator<> &alloc)
{
    PGSqlString sql("select ");
    sql += tableFields.id + ", ";
    sql += tableFields.received_datetime;
    sql += " from ";
    sql += tableNames.t0008_message_received;
    sql.addCompare(" where ", tableFields.receiver_id, " = ", receiver_id);
    sql.addCompare(" and ", tableFields.received_datetime, " > ", since);
    PGExecutor e(pool,
                 sql);
    e.toJsonArray(target, alloc);
    return true;
}

bool DatabaseLogicMessages::fetchReadMessages(const sole::uuid &reader_id,
                                              const std::chrono::system_clock::time_point &since,
                                              rapidjson::Value &target, rapidjson::MemoryPoolAllocator<> &alloc)
{
    PGSqlString sql("select ");
    sql += tableFields.id + ", ";
    sql += tableFields.read_datetime;
    sql += " from ";
    sql += tableNames.t0014_message_read;
    sql.addCompare(" where ", tableFields.reader_id, " = ", reader_id);
    sql.addCompare(" and ", tableFields.read_datetime, " > ", since);
    PGExecutor e(pool,
                 sql);
    e.toJsonArray(target, alloc);
    return true;
}

void DatabaseLogicMessages::setReceived(const sole::uuid &receiver_id,
                                        const sole::uuid &message_id,
                                        const std::chrono::system_clock::time_point &received_datetime)
{
    PGSqlString sql;
    sql.insert(tableNames.t0008_message_received);
    sql.addInsert(tableFields.id, sole::uuid4());
    sql.addInsert(tableFields.receiver_id, receiver_id);
    sql.addInsert(tableFields.message_id, message_id);
    sql.addInsert(tableFields.read_datetime, received_datetime);
    PGSqlString update(" update ");
    update.addSet(tableFields.read_datetime, received_datetime);
    sql.addOnConflict(tableFields.message_id + ", " + tableFields.reader_id, update);

    PGExecutor e(pool,
                 sql);
}

void DatabaseLogicMessages::setRead(const sole::uuid &reader_id,
                                    const sole::uuid &message_id,
                                    const std::chrono::system_clock::time_point &read_datetime)
{
    PGSqlString sql;
    sql.insert(tableNames.t0014_message_read);
    sql.addInsert(tableFields.id, sole::uuid4());
    sql.addInsert(tableFields.reader_id, reader_id);
    sql.addInsert(tableFields.message_id, message_id);
    sql.addInsert(tableFields.read_datetime, read_datetime);
    PGSqlString update(" do update set ");
    update.addSet(tableFields.read_datetime, read_datetime);
    sql.addOnConflict(tableFields.message_id + ", " + tableFields.reader_id, update);

    PGExecutor e(pool,
                 sql);
}
