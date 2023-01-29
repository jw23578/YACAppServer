#include "databaselogictables.h"
#include "postgres/pgconnection.h"
#include "postgres/pgexecutor.h"

DatabaseLogicTables::DatabaseLogicTables(LogStatController &logStatController,
                                         PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    utils(pool)
{

}

bool DatabaseLogicTables::connectionOk()
{
    try
    {
        PGConnection connection(pool);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool DatabaseLogicTables::pgCryptoInstalled()
{
    return utils.pgCryptoInstalled();
}

void DatabaseLogicTables::createDatabaseTables()
{
    if (!utils.tableExists(tableNames.t0001_users))
    {
        PGSqlString sql("create table ");
        sql += tableNames.t0001_users;
        sql += std::string(" ( id uuid, "
                           " loginemail text, "
                           " password_hash text, "
                           " verified timestamp, "
                           " verify_token text, "
                           " verify_token_valid_until timestamp with time zone, "
                           " login_token text, "
                           " login_token_valid_until timestamp with time zone,"
                           " primary key (id)) ");
        PGExecutor e(pool, sql);
    }
    std::string t0001_user_i1("t0001_user_i1");
    utils.createIndex(tableNames.t0001_users, t0001_user_i1, "(loginemail)");

    if (!utils.tableExists(tableNames.t0002_apps))
    {
        PGSqlString sql("create table ");
        sql += tableNames.t0002_apps;
        sql += std::string("( id uuid, "
                           "owner_id uuid, "
                           "app_name text, "
                           "app_version int, "
                           "app_logo_url text, "
                           "app_color_name text, "
                           "is_template_app bool, "
                           "json_yacapp text, "
                           "yacpck_base64 oid, "
                           "primary key (id))");
        PGExecutor e(pool, sql);
    }

    utils.createTableIfNeeded(tableNames.t0003_appuser_profiles,
                              {{tableFields.id, pg_uuid, true},
                               {tableFields.app_id, pg_uuid, false, true},
                               {tableFields.fstname, pg_text},
                               {tableFields.surname, pg_text},
                               {tableFields.visible_name, pg_text},
                               {"loginemail", pg_text, false, true},
                               {"verified", pg_timestamp},
                               {"verify_token", pg_text},
                               {"verify_token_valid_until", pg_timestamp},
                               {"update_password_token", pg_text},
                               {"update_password_token_valid_until", pg_timestamp},
                               {"deleted", pg_timestamp},
                               {"searching_exactly_allowed", pg_bool},
                               {"searching_fuzzy_allowed", pg_bool},
                               {tableFields.public_key_base64, pg_text},
                               {tableFields.image_id, pg_uuid}});

    utils.createTableIfNeeded(tableNames.t0009_appuser_logintoken,
                              {{tableFields.id, pg_uuid, true},
                               {"appuser_id", pg_uuid, false, true},
                               {"login_token", pg_text},
                               {"login_token_valid_until", pg_timestamp}});

    utils.createTableIfNeeded(tableNames.t0004_appuser_passwordhashes,
                              {{tableFields.id, pg_uuid, true},
                               {"appuser_id", pg_uuid, false, true},
                               {"password_hash", pg_text}});

    utils.createTableIfNeeded(tableNames.t0005_group_of_appusers,
                              {{tableFields.id, pg_uuid, true},
                               {"name", pg_text}});

    utils.createTableIfNeeded(tableNames.t0006_appuser2group,
                              {{tableFields.id, pg_uuid, true},
                               {"appuser_id", pg_uuid, false, true},
                               {"group_id", pg_uuid, false, true}});

    utils.createTableIfNeeded(tableNames.t0007_messages,
                              {{tableFields.id, pg_uuid, true},
                               {"sender_id", pg_uuid, false, true},
                               {"to_id", pg_uuid, false, true},
                               {"sended_datetime", pg_timestamp},
                               {"content_base64", pg_text}});

    utils.createTableIfNeeded(tableNames.t0008_message_received,
                              {{tableFields.id, pg_uuid, true},
                               {tableFields.message_id, pg_uuid, false, true},
                               {tableFields.receiver_id, pg_uuid, false, true},
                               {tableFields.received_datetime, pg_timestamp}},
                              {{tableFields.message_id, tableFields.receiver_id}});

    utils.createTableIfNeeded(tableNames.t0014_message_read,
                              {{tableFields.id, pg_uuid, true},
                               {tableFields.message_id, pg_uuid, false, true},
                               {tableFields.reader_id, pg_uuid, false, true},
                               {tableFields.read_datetime, pg_timestamp}},
                              {{tableFields.message_id, tableFields.reader_id}});

    const std::string creater_id("creater_id");
    const std::string created("created");
    const std::string finished("finished");
    utils.createTableIfNeeded(tableNames.t0010_task,
                              {{tableFields.id, pg_uuid, true},
                               {tableFields.task, pg_text},
                               {creater_id, pg_uuid, false, true},
                               {created, pg_timestamp},
                               {finished, pg_timestamp}});

    utils.createTableIfNeeded(tableNames.t0011_task_time,
                              {{tableFields.id, pg_uuid, true},
                               {tableFields.user_id, pg_uuid, false, true},
                               {tableFields.task_start, pg_timestamp},
                               {tableFields.task_end, pg_timestamp}});
    const std::string ts("ts");
    const std::string type("type");
    utils.createTableIfNeeded(tableNames.t0012_worktime,
                              {{tableFields.id, pg_uuid, true},
                               {tableFields.user_id, pg_uuid, false, true},
                               {ts, pg_timestamp, false, true},
                               {type, pg_int, false, true}});

    utils.createTableIfNeeded(tableNames.t0013_images,
                              {{tableFields.id, pg_uuid, true},
                               {tableFields.data, pg_blob}});

    utils.createTableIfNeeded(tableNames.t0015_appuser_devicetoken,
                              {{tableFields.id, pg_uuid, true},
                               {tableFields.user_id, pg_uuid, false, true},
                               {tableFields.device_token, pg_text}});
}

