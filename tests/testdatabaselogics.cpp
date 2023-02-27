#include "testdatabaselogics.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

TestDatabaseLogics::TestDatabaseLogics(DatabaseLogics &databaseLogics)
{
    DatabaseLogicAppointments &dla(databaseLogics.databaseLogicAppointments);
    sole::uuid id(sole::uuid4());
    sole::uuid owner_id(sole::uuid4());
    dla.insertAppointmentTemplate(id,
                                  "termin vorlage",
                                  30,
                                  0,
                                  owner_id);

    dla.updateAppointmentTemplate(id,
                                  "neue termin vorlage",
                                  45,
                                  1,
                                  id);

    dla.updateAppointmentTemplate(id,
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
    dlrg.insertRightGroup(id, "group #1", creater_id, message);
    dlrg.updateRightGroup(id, "right group with new name", id, message);
    dlrg.updateRightGroup(id, "right group with really new name", creater_id, message);
    rapidjson::Value rightGroups;
    dlrg.fetchRightGroups(rightGroups, document.GetAllocator(), message);
    document.AddMember("rightGroups", rightGroups, document.GetAllocator());
    dlrg.deleteRightGroup(id, id, message);
    dlrg.deleteRightGroup(id, creater_id, message);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, buffer.GetString());
}
