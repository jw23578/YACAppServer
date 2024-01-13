#include "databaselogictables.h"
#include "postgres/pgconnection.h"
#include "postgres/pgexecutor.h"

DatabaseLogicTables::DatabaseLogicTables(LogStatController &logStatController,
                                         PGConnectionPool &pool,
                                         YACORMFactory &factory):
    logStatController(logStatController),
    pool(pool),
    factory(factory),
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
    PGColumnAndType idPrimaryKey({tableFields.id, pg_uuid, true});
    PGColumnAndType appId({tableFields.app_id, pg_uuid, false, true});
    PGColumnAndType deletedDateTime({tableFields.deleted_datetime, pg_timestamp});
    PGColumnAndType deletedAppUser({tableFields.deleted_appuser_id, pg_uuid});
    PGColumnAndType accessCode({tableFields.access_code, pg_text});
    PGColumnAndType requestAllowed({tableFields.request_allowed, pg_bool});
    PGColumnAndType resultSeen({tableFields.result_seen, pg_timestamp});
    PGColumnAndType appuserId({tableFields.appuser_id, pg_uuid, false, true});
    PGColumnAndType requestedDatetime({tableFields.requested_datetime, pg_timestamp});
    PGColumnAndType approvedDatetime({tableFields.approved_datetime, pg_timestamp});
    PGColumnAndType approvedAppuserId({tableFields.approved_appuser_id, pg_uuid});
    PGColumnAndType deniedDatetime({tableFields.denied_datetime, pg_timestamp});
    PGColumnAndType deniedAppuserId({tableFields.denied_appuser_id, pg_uuid});
    PGColumnAndType visibleForNonMembers({tableFields.visible_for_non_members, pg_bool});
    PGColumnAndType sender_id({tableFields.sender_id, pg_uuid, false, true});

    for (const auto &on: factory.getORMNames())
    {
        std::unique_ptr<YACBaseObject> object(factory.create(on));
        std::vector<PGColumnAndType> columnsAndTypes;
        columnsAndTypes.push_back(idPrimaryKey);
        for (const auto &pn: object->propertyNames())
        {
            if (pn != tableFields.id && !object->isTransferProperty(pn))
            {
                if (object->propertyIsBool(pn))
                {
                    columnsAndTypes.push_back({pn, pg_bool, false, object->shouldBeIndexed(pn)});
                }
                if (object->propertyIsUuid(pn))
                {
                    columnsAndTypes.push_back({pn, pg_uuid, false, object->shouldBeIndexed(pn)});
                }
                if (object->propertyIsDateTime(pn))
                {
                    columnsAndTypes.push_back({pn, pg_timestamp, false, object->shouldBeIndexed(pn)});
                }
                if (object->propertyIsString(pn))
                {
                    columnsAndTypes.push_back({pn, pg_text, false, object->shouldBeIndexed(pn)});
                }
                if (object->propertyIsInt(pn))
                {
                    columnsAndTypes.push_back({pn, pg_bigint, false, object->shouldBeIndexed(pn)});
                }
                if (object->propertyIsOid(pn))
                {
                    columnsAndTypes.push_back({pn, pg_blob, false, object->shouldBeIndexed(pn)});
                }
            }
        }
        utils.createTableIfNeeded(on, columnsAndTypes);
    }



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

    utils.createTableIfNeeded(tableNames.t0009_appuser_logintoken,
                              {idPrimaryKey,
                               {tableFields.appuser_id, pg_uuid, false, true},
                               {"login_token", pg_text},
                               {tableFields.login_token_valid_until, pg_timestamp}});

    utils.createTableIfNeeded(tableNames.t0004_appuser_passwordhashes,
                              {idPrimaryKey,
                               {tableFields.appuser_id, pg_uuid, false, true},
                               {tableFields.password_hash, pg_text}});

    utils.createTableIfNeeded(tableNames.t0005_group_of_appusers,
                              {idPrimaryKey,
                               {tableFields.name, pg_text}});

    utils.createTableIfNeeded(tableNames.t0006_appuser2group,
                              {idPrimaryKey,
                               {tableFields.appuser_id, pg_uuid, false, true},
                               {"group_id", pg_uuid, false, true}});

    utils.createTableIfNeeded(tableNames.t0007_messages,
                              {{tableFields.id, pg_uuid, true},
                               sender_id,
                               {tableFields.to_id, pg_uuid, false, true},
                               {tableFields.sended_datetime, pg_timestamp},
                               {tableFields.content_base64, pg_text},
                               deletedDateTime});

    utils.createTableIfNeeded(tableNames.t0008_message_received,
                              {idPrimaryKey,
                               {tableFields.message_id, pg_uuid, false, true},
                               {tableFields.receiver_id, pg_uuid, false, true},
                               {tableFields.received_datetime, pg_timestamp}},
                              {{tableFields.message_id, tableFields.receiver_id}});

    utils.createTableIfNeeded(tableNames.t0014_message_read,
                              {idPrimaryKey,
                               {tableFields.message_id, pg_uuid, false, true},
                               {tableFields.reader_id, pg_uuid, false, true},
                               {tableFields.read_datetime, pg_timestamp}},
                              {{tableFields.message_id, tableFields.reader_id}});

    utils.createTableIfNeeded(tableNames.t0010_task,
                              {idPrimaryKey,
                               {tableFields.task, pg_text},
                               {tableFields.creater_id, pg_uuid, false, true},
                               {tableFields.created, pg_timestamp},
                               {tableFields.finished, pg_timestamp}});

    utils.createTableIfNeeded(tableNames.t0011_task_time,
                              {idPrimaryKey,
                               {tableFields.user_id, pg_uuid, false, true},
                               {tableFields.task_start, pg_timestamp},
                               {tableFields.task_end, pg_timestamp}});

    utils.createTableIfNeeded(tableNames.t0012_worktime,
                              {idPrimaryKey,
                               {tableFields.user_id, pg_uuid, false, true},
                               {tableFields.ts, pg_timestamp, false, true},
                               {tableFields.type, pg_int, false, true},
                               {tableFields.user_mood, pg_int},
                               {tableFields.day_rating, pg_int},
                               deletedDateTime,
                               deletedAppUser});

    utils.createTableIfNeeded(tableNames.t0013_images,
                              {{tableFields.id, pg_uuid, true},
                               {tableFields.data, pg_blob}});

    utils.createTableIfNeeded(tableNames.t0015_appuser_devicetoken,
                              {idPrimaryKey,
                               {tableFields.user_id, pg_uuid, false, true},
                               {tableFields.device_token, pg_text}});

    utils.createTableIfNeeded(tableNames.t0016_appointment_templates,
                              {idPrimaryKey,
                               appId,
                               {tableFields.name, pg_text},
                               {tableFields.default_duration_in_minutes, pg_int},
                               {tableFields.color, pg_int},
                               {tableFields.owner_id, pg_uuid}});

    utils.createTableIfNeeded(tableNames.t0017_element_usable4appuser,
                              {idPrimaryKey,
                               {tableFields.element_id, pg_uuid, false, true},
                               {tableFields.appuser_id, pg_uuid, false, true}});

    utils.createTableIfNeeded(tableNames.t0018_appointment,
                              {idPrimaryKey,
                               appId,
                               {tableFields.appointment_group_id, pg_uuid, false, true},
                               {tableFields.appointment_template_id, pg_uuid, false, true},
                               {tableFields.caption, pg_text},
                               {tableFields.decription, pg_text},
                               {tableFields.start_datetime, pg_timestamp},
                               {tableFields.end_datetime, pg_timestamp},
                               {tableFields.creater_id, pg_uuid, false, true},
                               {tableFields.deleted_datetime, pg_timestamp},
                               {tableFields.history_datetime, pg_timestamp},
                               {tableFields.max_bookable_count, pg_int},
                               {tableFields.bookable_since_datetime, pg_timestamp},
                               {tableFields.bookable_until_datetime, pg_timestamp},
                               {tableFields.booking_credits, pg_int},
                               {tableFields.visible_for_everybody, pg_bool},
                               {tableFields.no_space, pg_bool}});

    utils.createTableIfNeeded(tableNames.t0019_element_visible4appuser,
                              {idPrimaryKey,
                               {tableFields.element_id, pg_uuid, false, true},
                               {tableFields.appuser_id, pg_uuid, false, true}});

    utils.createTableIfNeeded(tableNames.t0020_appointment_participants,
                              {idPrimaryKey,
                               {tableFields.appointment_id, pg_uuid, false, true},
                               {tableFields.appuser_id, pg_uuid, false, true}});

    utils.createTableIfNeeded(tableNames.t0024_space,
                              {idPrimaryKey,
                               appId,
                               {tableFields.name, pg_text, false, true},
                               {tableFields.creater_id, pg_uuid, false, true},
                               deletedDateTime,
                               deletedAppUser,
                               {tableFields.automatic, pg_bool},
                               accessCode,
                               requestAllowed});

    utils.createTableIfNeeded(tableNames.t0025_space2appuser,
                              {idPrimaryKey,
                               appId,
                               {tableFields.space_id, pg_uuid, false, true},
                               appuserId,
                               requestedDatetime,
                               approvedDatetime,
                               approvedAppuserId,
                               deniedDatetime,
                               deniedAppuserId,
                               resultSeen});

}

