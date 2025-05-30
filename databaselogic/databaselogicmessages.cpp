#include "databaselogicmessages.h"
#include "pgexecutor.h"
#include "JWUtils/definitions.h"
#include "pgoidstorer.h"
#include "pgoidloader.h"
#include "base64.h"
#include "orm-mapper/orm2postgres.h"

DatabaseLogicMessages::DatabaseLogicMessages(LogStatController &logStatController,
                                             PGConnectionPool &pool):
    DatabaseLogicInterface(logStatController, pool)
{

}

void DatabaseLogicMessages::storeMessage(const reducedsole::uuid &id,
                                         const reducedsole::uuid &sender_id,
                                         const reducedsole::uuid &to_id,
                                         const std::string &content_base64)
{
    SqlString sql(utils.createInsertString(tableNames.t0007_messages));
    MACRO_set(sql, id);
    MACRO_set(sql, sender_id);
    MACRO_set(sql, to_id);
    MACRO_set(sql, content_base64);
    sql.set(tableFields.sended_datetime, TimePointPostgreSqlNow);
    sql.set(tableFields.deleted_datetime, TimePointPostgreSqlNull);
    PGExecutor e(pool, sql);
}

void DatabaseLogicMessages::deleteMessage(const reducedsole::uuid &id)
{
    SqlString sql;
    sql.delet(tableNames.t0007_messages);
    sql.addCompare("where", tableFields.id, "=", id);
    PGExecutor e(pool, sql);
}

bool DatabaseLogicMessages::markAllOfUserMessageDeleted(const reducedsole::uuid &user_id,
                                                        std::string &resultMessage)
{
    SqlString upd;
    upd.update(tableNames.t0007_messages);
    upd.addSet(tableFields.deleted_datetime, TimePointPostgreSqlNow, false);
    upd.addCompare("where (", tableFields.sender_id, "=", user_id);
    upd.addCompare("or", tableFields.to_id, "=", user_id);
    upd.addCompare(") and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    PGExecutor e(pool, upd, resultMessage, "messsages deleted", "no message found");
    return e.affected_rows() > 0;
}

bool DatabaseLogicMessages::markMessageDeleted(const reducedsole::uuid &id,
                                               const reducedsole::uuid &sender_id,
                                               std::string &resultMessage)
{
    SqlString upd;
    upd.update(tableNames.t0007_messages);
    upd.addSet(tableFields.deleted_datetime, TimePointPostgreSqlNow, false);
    upd.addCompare("where", tableFields.id, "=", id);
    upd.addCompare("and", tableFields.sender_id, "=", sender_id);
    PGExecutor e(pool, upd, resultMessage, "messsage deleted", "message not found");
    return e.affected_rows() > 0;
}

bool DatabaseLogicMessages::fetchMessages(const reducedsole::uuid &fetcher_id,
                                          const std::chrono::system_clock::time_point &since,
                                          rapidjson::Value &target,
                                          rapidjson::MemoryPoolAllocator<> &alloc)
{
    SqlString sql("select * from ");
    sql += tableNames.t0007_messages;
    sql += " where (sender_id = :fetcher_id or to_id = :fetcher_id ";
    sql += " or to_id in (select group_id from  ";
    sql += tableNames.t0006_appuser2group;
    sql += " where appuser_id = :fetcher_id)) ";
    sql += " and (sended_datetime >= :since or deleted_datetime >= :since) ";
    MACRO_set(sql, fetcher_id);
    MACRO_set(sql, since);
    ORM2Postgres o2p(pool);
    o2p.toJsonArray(sql, target, alloc);
    return true;
}

bool DatabaseLogicMessages::fetchReceivedMessages(const reducedsole::uuid &receiver_id,
                                                  const std::chrono::system_clock::time_point &since,
                                                  rapidjson::Value &target,
                                                  rapidjson::MemoryPoolAllocator<> &alloc)
{
    SqlString sql("select ");
    sql += tableFields.id + ", ";
    sql += tableFields.received_datetime;
    sql += " from ";
    sql += tableNames.t0008_message_received;
    sql.addCompare(" where ", tableFields.receiver_id, " = ", receiver_id);
    sql.addCompare(" and ", tableFields.received_datetime, " > ", since);
    ORM2Postgres o2p(pool);
    o2p.toJsonArray(sql, target, alloc);
    return true;
}

bool DatabaseLogicMessages::fetchReadMessages(const reducedsole::uuid &reader_id,
                                              const std::chrono::system_clock::time_point &since,
                                              rapidjson::Value &target, rapidjson::MemoryPoolAllocator<> &alloc)
{
    SqlString sql("select ");
    sql += tableFields.id + ", ";
    sql += tableFields.read_datetime;
    sql += " from ";
    sql += tableNames.t0014_message_read;
    sql.addCompare(" where ", tableFields.reader_id, " = ", reader_id);
    sql.addCompare(" and ", tableFields.read_datetime, " > ", since);
    ORM2Postgres o2p(pool);
    o2p.toJsonArray(sql, target, alloc);
    return true;
}

bool DatabaseLogicMessages::fetchReceivedAndReadMessages(const reducedsole::uuid &receiver_or_reader_id,
                                                         const std::chrono::system_clock::time_point &since,
                                                         rapidjson::Value &targetReceived,
                                                         rapidjson::Value &targetRead,
                                                         rapidjson::MemoryPoolAllocator<> &alloc)
{
    SqlString sql("select '1' as type, ");
    sql += tableFields.id + ", ";
    sql += tableFields.received_datetime;
    sql += " from ";
    sql += tableNames.t0008_message_received;
    sql.addCompare(" where ", tableFields.receiver_id, " = ", receiver_or_reader_id);
    sql.addCompare(" and ", tableFields.received_datetime, " > ", since);
    sql += " union all ";
    sql += "select '2' as type, ";
    sql += tableFields.id + ", ";
    sql += tableFields.read_datetime;
    sql += " from ";
    sql += tableNames.t0014_message_read;
    sql.addCompare(" where ", tableFields.reader_id, " = ", receiver_or_reader_id);
    sql.addCompare(" and ", tableFields.read_datetime, " > ", since);
    PGExecutor e(pool,
                 sql);
    ORM2Postgres o2p(pool);
    while (e.resultAvailable())
    {
        int type(e.integer("type"));
        rapidjson::Value object(rapidjson::kObjectType);
        o2p.toJsonObject(e, object, alloc);
        if (type == 1)
        {
            targetReceived.PushBack(object, alloc);
        }
        if (type == 2)
        {
            targetRead.PushBack(object, alloc);
        }
        e.next();
    }
    return true;
}

void DatabaseLogicMessages::setReceived(const reducedsole::uuid &receiver_id,
                                        const reducedsole::uuid &message_id,
                                        const std::chrono::system_clock::time_point &received_datetime)
{
    SqlString sql;
    sql.insert(tableNames.t0008_message_received);
    sql.addInsert(tableFields.id, reducedsole::uuid4(), false);
    sql.addInsert(tableFields.receiver_id, receiver_id, false);
    sql.addInsert(tableFields.message_id, message_id, false);
    sql.addInsert(tableFields.read_datetime, received_datetime, false);
    SqlString update(" update ");
    update.addSet(tableFields.read_datetime, received_datetime, false);
    sql.addOnConflict(tableFields.message_id + ", " + tableFields.reader_id, update);

    PGExecutor e(pool,
                 sql);
}

void DatabaseLogicMessages::setRead(const reducedsole::uuid &reader_id,
                                    const reducedsole::uuid &message_id,
                                    const std::chrono::system_clock::time_point &read_datetime)
{
    SqlString sql;
    sql.insert(tableNames.t0014_message_read);
    sql.addInsert(tableFields.id, reducedsole::uuid4(), false);
    sql.addInsert(tableFields.reader_id, reader_id, false);
    sql.addInsert(tableFields.message_id, message_id, false);
    sql.addInsert(tableFields.read_datetime, read_datetime, false);
    SqlString update(" do update set ");
    update.addSet(tableFields.read_datetime, read_datetime, false);
    sql.addOnConflict(tableFields.message_id + ", " + tableFields.reader_id, update);

    PGExecutor e(pool,
                 sql);
}

bool DatabaseLogicMessages::storeImage(t0028_message_images &t0028)
{
    pqxx::oid oid;
    std::vector<char> data;
    bin_base64_decode(t0028.transfer_image_base64, data);
    std::basic_string<std::byte> image_data(reinterpret_cast<std::byte*>(data.data()), data.size());
    PGOidStorer storeOid(pool, image_data, oid);
    t0028.image_oid = oid;
    ORM2Postgres orm2postgres(pool);
    orm2postgres.insert(t0028);
    return true;
}

bool DatabaseLogicMessages::lookUpOid(const reducedsole::uuid &imageId,
                                        pqxx::oid &imageOid)
{
    PGExecutor e(pool);
    e.select(tableNames.t0028_message_images,
             tableFields.id,
             imageId);
    if (!e.size())
    {
        return false;
    }
    imageOid = e.oid(tableFields.image_oid);
    return true;
}

bool DatabaseLogicMessages::fetchImage(const reducedsole::uuid &imageId,
                                       std::string &message,
                                       std::basic_string<std::byte> &data)
{
    auto it(imageId2oid.find(imageId));
    pqxx::oid imageOid(0);
    if (it == imageId2oid.end())
    {
        if (!lookUpOid(imageId, imageOid))
        {
            message = "could not look up image oid for imageid: " + ExtString::toString(imageId);
            return false;
        }
    }
    else
    {
        imageOid = it->second;
    }

    PGOidLoader loader(pool, imageOid, data);
    if (data.size() == 0)
    {
        message = "could not load image with oid: " + ExtString::toString(imageOid);
        return false;
    }
    return true;
}
