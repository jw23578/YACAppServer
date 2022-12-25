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
                              {{"id", pg_uuid, true},
                               {"app_id", pg_uuid, false, true},
                               {"fstname", pg_text},
                               {"surname", pg_text},
                               {"visible_name", pg_text},
                               {"loginemail", pg_text, false, true},
                               {"verified", pg_timestamp},
                               {"verify_token", pg_text},
                               {"verify_token_valid_until", pg_timestamp},
                               {"update_password_token", pg_text},
                               {"update_password_token_valid_until", pg_timestamp},
                               {"deleted", pg_timestamp},
                               {"searching_exactly_allowed", pg_bool},
                               {"searching_fuzzy_allowed", pg_bool},
                               {"public_key_base64", pg_text},
                               {"image_id", pg_uuid}});

    utils.createTableIfNeeded(tableNames.t0009_appuser_logintoken,
                              {{"id", pg_uuid, true},
                               {"appuser_id", pg_uuid, false, true},
                               {"login_token", pg_text},
                               {"login_token_valid_until", pg_timestamp}});

    utils.createTableIfNeeded(tableNames.t0004_appuser_passwordhashes,
                              {{"id", pg_uuid, true},
                               {"appuser_id", pg_uuid, false, true},
                               {"password_hash", pg_text}});

    utils.createTableIfNeeded(tableNames.t0005_group_of_appusers,
                              {{"id", pg_uuid, true},
                               {"name", pg_text}});

    utils.createTableIfNeeded(tableNames.t0006_appuser2group,
                              {{"id", pg_uuid, true},
                               {"appuser_id", pg_uuid, false, true},
                               {"group_id", pg_uuid, false, true}});

    utils.createTableIfNeeded(tableNames.t0007_messages,
                              {{"id", pg_uuid, true},
                               {"sender_id", pg_uuid, false, true},
                               {"to_id", pg_uuid, false, true},
                               {"sended_datetime", pg_timestamp},
                               {"content_base64", pg_text}});

    utils.createTableIfNeeded(tableNames.t0008_message_state,
                              {{"id", pg_uuid, true},
                               {"message_id", pg_uuid, false, true},
                               {"receiver_id", pg_uuid, false, true},
                               {"received_datetime", pg_timestamp},
                               {"read_datetime", pg_timestamp}});

    const std::string id("id");
    const std::string task("task");
    const std::string creater_id("creater_id");
    const std::string created("created");
    const std::string finished("finished");
    utils.createTableIfNeeded(tableNames.t0010_task,
                              {{id, pg_uuid, true},
                               {task, pg_text},
                               {creater_id, pg_uuid, false, true},
                               {created, pg_timestamp},
                               {finished, pg_timestamp}});

    const std::string user_id("user_id");
    const std::string start("start");
    const std::string end("end");
    utils.createTableIfNeeded(tableNames.t0011_task_time,
                              {{id, pg_uuid, true},
                               {user_id, pg_uuid, false, true},
                               {start, pg_timestamp},
                               {end, pg_timestamp}});
    const std::string ts("ts");
    const std::string type("type");
    utils.createTableIfNeeded(tableNames.t0012_worktime,
                              {{id, pg_uuid, true},
                               {user_id, pg_uuid, false, true},
                               {ts, pg_timestamp, false, true},
                               {type, pg_int, false, true}});

    const std::string data("data");
    utils.createTableIfNeeded(tableNames.t0013_images,
                              {{id, pg_uuid, true},
                               {data, pg_blob}});
}

