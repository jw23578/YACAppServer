#include "testdatabaselogics.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

TestDatabaseLogics::TestDatabaseLogics(DatabaseLogics &databaseLogics)
{
    DatabaseLogicAppointments &dla(databaseLogics.databaseLogicAppointments);
    sole::uuid id(sole::uuid4());
    sole::uuid app_id(sole::uuid4());
    sole::uuid owner_id(sole::uuid4());
    dla.insertAppointmentTemplate(id,
                                  app_id,
                                  "termin vorlage",
                                  30,
                                  0,
                                  owner_id);

    dla.updateAppointmentTemplate(id,
                                  app_id,
                                  "neue termin vorlage",
                                  45,
                                  1,
                                  id);

    dla.updateAppointmentTemplate(id,
                                  app_id,
                                  "neue termin vorlage",
                                  45,
                                  1,
                                  owner_id);

    std::string message;
    dla.deleteAppointmentTemplate(id,
                                  owner_id,
                                  message);

    sole::uuid creater_id(sole::uuid4());
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value appointment;
    bool visible_for_everybody(true);
    dla.insertAppointment(id,
                          app_id,
                          id,
                          id,
                          "appointment",
                          "description",
                          std::chrono::system_clock::now(),
                          std::chrono::system_clock::now(),
                          creater_id,
                          0,
                          std::chrono::system_clock::now(),
                          std::chrono::system_clock::now(),
                          0,
                          visible_for_everybody,
                          appointment,
                          document.GetAllocator(),
                          message);

    dla.deleteAppointment(id, id, message);
    dla.deleteAppointment(id, creater_id, message);

    DatabaseLogicRightGroup &dlrg(databaseLogics.databaseLogicRightGroup);
    rapidjson::Value rightGroup;
    bool automatic(false);
    id = NullUuid;
    bool request_allowed(false);
    bool visible_for_non_members(false);
    dlrg.insertOrUpdateRightGroup(id, app_id, "group #1", creater_id, automatic, "1234", request_allowed, visible_for_non_members, rightGroup, document.GetAllocator(), message);
    dlrg.insertOrUpdateRightGroup(id, app_id, "right group with new name", id, automatic, "1234", request_allowed, visible_for_non_members, rightGroup, document.GetAllocator(), message);
    dlrg.insertOrUpdateRightGroup(id, app_id, "right group with really new name", creater_id, automatic, "1234", request_allowed, visible_for_non_members, rightGroup, document.GetAllocator(), message);
    rapidjson::Value rightGroups;
    dlrg.fetchRightGroups(app_id, rightGroups, document.GetAllocator(), message);
    document.AddMember("rightGroups", rightGroups, document.GetAllocator());
    dlrg.deleteRightGroup(id, id, message);
    dlrg.deleteRightGroup(id, creater_id, message);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, buffer.GetString());
}
