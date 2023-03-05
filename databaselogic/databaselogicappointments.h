#ifndef DATABASELOGICAPPOINTMENTS_H
#define DATABASELOGICAPPOINTMENTS_H

#include "postgres/pgconnectionpool.h"
#include "logstat/logstatcontroller.h"
#include "sole/sole.hpp"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"

class DatabaseLogicAppointments
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    TableNames tableNames;
    TableFields tableFields;
    bool checkAppointmentTemplateOwner(const sole::uuid &id,
                                       const sole::uuid &owner_id,
                                       std::string &message);
    bool checkAppointmentCreater(const sole::uuid &id,
                                 const sole::uuid &creater_id,
                                 std::string &message);
    bool fetchOneAppointment(const sole::uuid &id,
                             rapidjson::Value &object,
                             rapidjson::MemoryPoolAllocator<> &alloc,
                             std::string &message);
public:
    DatabaseLogicAppointments(LogStatController &logStatController,
                              PGConnectionPool &pool);

    bool insertAppointmentTemplate(sole::uuid const &id,
                                   const sole::uuid &app_id,
                                   std::string const &name,
                                   int default_duration_in_minutes,
                                   int color,
                                   sole::uuid const &owner_id);
    bool updateAppointmentTemplate(sole::uuid const &id,
                                   const sole::uuid &app_id,
                                   std::string const &name,
                                   int default_duration_in_minutes,
                                   int color,
                                   sole::uuid const &owner_id);
    bool deleteAppointmentTemplate(sole::uuid const &id,
                                   sole::uuid const &owner_id,
                                   std::string &message);

    bool fetchAppointmentTemplates(const sole::uuid &appuser_id,
                                   const sole::uuid &app_id,
                                   rapidjson::Value &target,
                                   rapidjson::MemoryPoolAllocator<> &alloc,
                                   std::string &message);

    bool insertAppointment(const sole::uuid &id,
                           const sole::uuid &app_id,
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
                           rapidjson::Value &target,
                           rapidjson::MemoryPoolAllocator<> &alloc,
                           std::string &message);

    bool deleteAppointment(const sole::uuid &id,
                           const sole::uuid &creater_id,
                           std::string &message);

    bool fetchAppointments(const sole::uuid &appuser_id,
                           const sole::uuid &app_id,
                           rapidjson::Value &target,
                           rapidjson::MemoryPoolAllocator<> &alloc,
                           std::string &message);
};

#endif // DATABASELOGICAPPOINTMENTS_H
