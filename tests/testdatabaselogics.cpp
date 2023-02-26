#include "testdatabaselogics.h"

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
                          message);

    dla.deleteAppointment(id, id, message);
    dla.deleteAppointment(id, creater_id, message);
}
