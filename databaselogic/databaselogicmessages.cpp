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

void DatabaseLogicMessages::fetchMessages(const sole::uuid &fetcher_id,
                                          const std::chrono::system_clock::time_point &since,
                                          std::vector<Message> &messages)
{
    PGSqlString sql("select * from ");
    sql += tableNames.t0007_messages;
    sql += " where (to_id = :fetcher_id ";
    sql += " or to_id in (select group_id from  ";
    sql += tableNames.t0006_appuser2group;
    sql += " where appuser_id = :fetcher_id)) ";
    sql += " and sended_datetime >= :since";
    MACRO_set(fetcher_id);
    MACRO_set(since);
    PGExecutor e(pool, sql);
    for (size_t i(0); i < e.size(); ++i)
    {
        Message m;
        m.id = e.uuid("id");
        m.sender_id = e.uuid("sender_id");
        m.sended_datetime = e.timepoint("sended_datetime");
        m.content_base64 = e.string("content_base64");
        messages.push_back(m);
        e.next();
    }
}

void DatabaseLogicMessages::setReceived(const sole::uuid &receiver_id,
                                        const std::set<sole::uuid> &message_ids)
{
    PGSqlString sql("select message_id from ");
    sql += tableNames.t0008_message_state;
    sql += " where receiver_id = :receiver_id ";
    sql += " and message_id in (:message_ids) ";
    MACRO_set(receiver_id);
    MACRO_set(message_ids);
    PGExecutor select(pool, sql);
    std::set<sole::uuid> message_ids2insert(message_ids);
    for (size_t i(0); i < select.size(); ++i)
    {
        message_ids2insert.erase(select.uuid("message_id"));
        select.next();
    }
    sql = utils.createInsertString(tableNames.t0008_message_state);
    for (const auto &message_id : message_ids2insert)
    {
        sql.set("id", sole::uuid4());
        sql.set("message_id", message_id);
        sql.set("receiver_id", receiver_id);
        sql.set("received_datetime", TimePointPostgreSqlNow);
        sql.set("read_datetime", TimePointPostgreSqlNull);
        PGExecutor e(pool, sql);
    }
}
