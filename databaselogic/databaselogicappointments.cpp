#include "databaselogicappointments.h"
#include "pgsqlstring.h"
#include "pgexecutor.h"

bool DatabaseLogicAppointments::checkAppointmentTemplateOwner(const sole::uuid &id,
                                                              const sole::uuid &owner_id,
                                                              std::string &message)
{
    PGExecutor e(pool);
    e.select(tableNames.t0016_appointment_templates, "id", id);
    if (!e.size())
    {
        message = "no appointment template found";
        return false;
    }
    if (e.size() > 1)
    {
        message = "fatal error, more than one template";
        return false;
    }
    MACRO_Uuid_NotEqual(e, owner_id)
    {
        message = "given owner ist not template owner";
        return false;
    }
    return true;
}

bool DatabaseLogicAppointments::checkAppointmentCreater(const sole::uuid &id,
                                                        const sole::uuid &creater_id,
                                                        std::string &message)
{
    PGExecutor e(pool);
    if (!e.defaultSelect(tableNames.t0018_appointment, id, message))
    {
        return false;
    }
    if (e.uuid(tableFields.creater_id) != creater_id)
    {
        message = "you are not the creater of this appointment";
        return false;
    }
    return true;
}

bool DatabaseLogicAppointments::fetchOneAppointment(const sole::uuid &id,
                                                    rapidjson::Value &object,
                                                    rapidjson::MemoryPoolAllocator<> &alloc,
                                                    std::string &message)
{
    PGExecutor e(pool);
    return e.defaultSelectToJSON(tableNames.t0018_appointment, id, object, alloc, message);
}

DatabaseLogicAppointments::DatabaseLogicAppointments(LogStatController &logStatController,
                                                     PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool)
{

}

bool DatabaseLogicAppointments::insertAppointmentTemplate(sole::uuid const &id,
                                                          const std::string &name,
                                                          int default_duration_in_minutes,
                                                          int color,
                                                          sole::uuid const &owner_id)
{
    PGSqlString insert;
    insert.insert(tableNames.t0016_appointment_templates);
    MACRO_addInsert(insert, id);
    MACRO_addInsert(insert, name);
    MACRO_addInsert(insert, default_duration_in_minutes);
    MACRO_addInsert(insert, color);
    MACRO_addInsert(insert, owner_id);
    PGExecutor e(pool, insert);
    return true;
}

bool DatabaseLogicAppointments::updateAppointmentTemplate(const sole::uuid &id,
                                                          const std::string &name,
                                                          int default_duration_in_minutes,
                                                          int color,
                                                          sole::uuid const &owner_id)
{
    std::string message;
    if (!checkAppointmentTemplateOwner(id, owner_id, message))
    {
        return false;
    }
    PGSqlString update;
    update.update(tableNames.t0016_appointment_templates);
    MACRO_addSet(update, name);
    MACRO_addSet(update, default_duration_in_minutes);
    MACRO_addSet(update, color);
    MACRO_addSet(update, owner_id);
    update.addCompare(" where ", tableFields.id, " = ", id);
    PGExecutor e(pool, update);
    return true;
}

bool DatabaseLogicAppointments::deleteAppointmentTemplate(const sole::uuid &id,
                                                          const sole::uuid &owner_id,
                                                          std::string &message)
{
    if (!checkAppointmentTemplateOwner(id, owner_id, message))
    {
        return false;
    }
    PGSqlString sql;
    sql.delet(tableNames.t0016_appointment_templates);
    sql.addCompare(" where ", tableFields.id, " = ", id);
    sql.addCompare(" and ", tableFields.owner_id, " = ", owner_id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicAppointments::fetchAppointmentTemplates(const sole::uuid &appuser_id,
                                                          rapidjson::Value &target,
                                                          rapidjson::MemoryPoolAllocator<> &alloc,
                                                          std::string &message)
{
    PGSqlString sql;
    sql.select(tableNames.t0016_appointment_templates);
    sql.addCompare(" where ", tableFields.owner_id, " = ", appuser_id);
    sql += " or id in (select element_id from t0019_element_visible4appuser where appuser_id = :appuser_id) ";
    sql.set("appuser_id", appuser_id);
    PGExecutor e(pool, sql);
    e.toJsonArray(target, alloc);
    return true;
}

bool DatabaseLogicAppointments::insertAppointment(const sole::uuid &id,
                                                  const sole::uuid &appointment_group_id,
                                                  const sole::uuid &appointment_template_id,
                                                  const std::string &caption,
                                                  const std::string &decription,
                                                  const std::chrono::system_clock::time_point &start_datetime,
                                                  const std::chrono::system_clock::time_point &end_datetime,
                                                  const sole::uuid &creater_id,
                                                  const int max_bookable_count,
                                                  const std::chrono::system_clock::time_point &bookable_since_datetime,
                                                  const std::chrono::system_clock::time_point &bookable_until_datetime,
                                                  const int booking_credits,
                                                  const bool visible_for_everybody,
                                                  rapidjson::Value &object,
                                                  rapidjson::MemoryPoolAllocator<> &alloc,
                                                  std::string &message)
{
    {
        PGSqlString sql;
        sql.select(tableNames.t0018_appointment);
        sql.addCompare(" where ", tableFields.appointment_group_id, " = ", appointment_group_id);
        sql.addCompare(" and ", tableFields.deleted_datetime, " = ", TimePointPostgreSqlNull);
        sql.addCompare(" and ", tableFields.history_datetime, " = ", TimePointPostgreSqlNull);
        PGExecutor e(pool, sql);
        if (e.size() > 1)
        {
            message = "fatal error, more than one active appointment with appointment_group_id: " + appointment_group_id.str();
            return false;
        }
        if (e.size() > 0)
        {
            if (e.uuid(tableFields.creater_id) != creater_id)
            {
                message = "only appuser with id: " + e.uuid(tableFields.creater_id).str() + " is allowed to update/insert this appointment";
                return false;
            }
            {
                const std::chrono::system_clock::time_point history_datetime(TimePointPostgreSqlNow);
                PGSqlString sql;
                sql.update(tableNames.t0018_appointment);
                MACRO_addSet(sql, history_datetime);
                sql.addCompare(" where ", tableFields.appointment_group_id, " = ", appointment_group_id);
            }
        }
    }
    const std::chrono::system_clock::time_point &deleted_datetime(TimePointPostgreSqlNull);
    const std::chrono::system_clock::time_point &history_datetime(TimePointPostgreSqlNull);
    PGSqlString sql;
    sql.insert(tableNames.t0018_appointment);
    MACRO_addInsert(sql, id);
    MACRO_addInsert(sql, appointment_group_id);
    MACRO_addInsert(sql, appointment_template_id);
    MACRO_addInsert(sql, caption);
    MACRO_addInsert(sql, decription);
    MACRO_addInsert(sql, start_datetime);
    MACRO_addInsert(sql, end_datetime);
    MACRO_addInsert(sql, creater_id);
    MACRO_addInsert(sql, deleted_datetime);
    MACRO_addInsert(sql, history_datetime);
    MACRO_addInsert(sql, max_bookable_count);
    MACRO_addInsert(sql, bookable_since_datetime);
    MACRO_addInsert(sql, bookable_until_datetime);
    MACRO_addInsert(sql, booking_credits);
    MACRO_addInsert(sql, visible_for_everybody);
    PGExecutor e(pool, sql);
    return fetchOneAppointment(id, object, alloc, message);
}

bool DatabaseLogicAppointments::deleteAppointment(const sole::uuid &id,
                                                  const sole::uuid &creater_id,
                                                  std::string &message)
{
    if (!checkAppointmentCreater(id, creater_id, message))
    {
        return false;
    }
    PGSqlString sql;
    sql.update(tableNames.t0018_appointment);
    const std::chrono::system_clock::time_point deleted_datetime(TimePointPostgreSqlNow);
    MACRO_addSet(sql, deleted_datetime);
    sql.addCompare("where", "id", "=", id);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicAppointments::fetchAppointments(const sole::uuid &appuser_id,
                                                  rapidjson::Value &target,
                                                  rapidjson::MemoryPoolAllocator<> &alloc,
                                                  std::string &message)
{
    PGSqlString sql;
    sql.select(tableNames.t0018_appointment);
    sql.addCompare(" where ( ", tableFields.creater_id, " = ", appuser_id);
    sql += std::string(" or ") + tableFields.visible_for_everybody;
    sql += std::string(") and ") + tableFields.deleted_datetime + std::string(" is null ");
    sql += std::string(" and ") + tableFields.history_datetime + std::string(" is null ");
    PGExecutor e(pool, sql);
    e.toJsonArray(target, alloc);
    return true;
}
