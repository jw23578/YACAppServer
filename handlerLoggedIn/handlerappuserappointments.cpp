#include "handlerappuserappointments.h"
#include "databaselogics.h"

HandlerAppUserAppointments::HandlerAppUserAppointments(DatabaseLogics &databaseLogics,
                                                       PistacheServerInterface &serverInterface,
                                                       LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             methodNames.fetchAppointmentTemplates,
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{
    addMethod(serverInterface,
              methodNames.insertAppointmentTemplate,
              TypePost);
    addMethod(serverInterface,
              methodNames.updateAppointmentTemplate,
              TypePost);
    addMethod(serverInterface,
              methodNames.deleteAppointmentTemplate,
              TypeDelete);
    addMethod(serverInterface,
              methodNames.insertAppointment,
              TypePost);
    addMethod(serverInterface,
              methodNames.deleteAppointment,
              TypeDelete);
    addMethod(serverInterface,
              methodNames.fetchAppointments,
              TypeGet);
}

void HandlerAppUserAppointments::method()
{
    DatabaseLogicAppointments &dla(databaseLogics.databaseLogicAppointments);
    if (isMethod(methodNames.fetchAppointmentTemplates))
    {
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value appointment_templates;
        std::string message;
        if (!dla.fetchAppointmentTemplates(loggedInUserId,
                                           appointment_templates,
                                           document.GetAllocator(),
                                           message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("appointment_templates", appointment_templates, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    if (isMethod(methodNames.fetchAppointments))
    {
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value appointments;
        std::string message;
        if (!dla.fetchAppointments(loggedInUserId,
                                   appointments,
                                   document.GetAllocator(),
                                   message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("appointments", appointments, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    if (isMethod(methodNames.deleteAppointment))
    {
        MACRO_GetMandatoryUuid(id);
        std::string message("appointment deleted");
        answerOk(message, dla.deleteAppointment(id, loggedInUserId, message));
        return;
    }
    if (isMethod(methodNames.insertAppointment))
    {
        MACRO_GetUuid(appointment_group_id);
        MACRO_GetUuid(appointment_template_id);
        if (appointment_group_id == NullUuid)
        {
            appointment_group_id = sole::uuid4();
        }
        if (appointment_template_id == NullUuid)
        {
            appointment_template_id = sole::uuid4();
        }
        MACRO_GetMandatoryString(caption);
        MACRO_GetString(decription);
        MACRO_GetMandatoryTimePointFromISO(start_datetime);
        MACRO_GetMandatoryTimePointFromISO(end_datetime);
        MACRO_GetMandatoryInt(max_bookable_count, true);
        MACRO_GetTimePointFromISO(bookable_since_datetime);
        MACRO_GetTimePointFromISO(bookable_until_datetime);
        MACRO_GetMandatoryInt(booking_credits, true);
        MACRO_GetMandatoryBool(visible_for_everybody);

        std::string message("appointment inserted");
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Value appointment;
        if (!dla.insertAppointment(sole::uuid4(),
                                   appointment_group_id,
                                   appointment_template_id,
                                   caption,
                                   decription,
                                   start_datetime,
                                   end_datetime,
                                   loggedInUserId,
                                   max_bookable_count,
                                   bookable_since_datetime,
                                   bookable_until_datetime,
                                   booking_credits,
                                   visible_for_everybody,
                                   appointment,
                                   document.GetAllocator(),
                                   message))
        {
            answerOk(message, false);
            return;
        }
        document.AddMember("appointment", appointment, document.GetAllocator());
        answerOk(true, document);
        return;
    }
    if (isMethod(methodNames.deleteAppointmentTemplate))
    {
        MACRO_GetMandatoryUuid(id);
        std::string message("appointmentTemplate deleted");
        answerOk(message, dla.deleteAppointmentTemplate(id, loggedInUserId, message));
        return;
    }
    MACRO_GetMandatoryString(name);
    MACRO_GetMandatoryInt(default_duration_in_minutes, false);
    MACRO_GetMandatoryInt(color, true);
    if (isMethod(methodNames.insertAppointmentTemplate))
    {
        if (!dla.insertAppointmentTemplate(sole::uuid4(),
                                           name,
                                           default_duration_in_minutes,
                                           color,
                                           loggedInUserId))
        {
            answerOk("could not insert appointment template", false);
            return;
        }
        answerOk("appointment template inserted", true);
        return;
    }
    if (isMethod(methodNames.updateAppointmentTemplate))
    {
        MACRO_GetMandatoryUuid(id);
        if (!dla.updateAppointmentTemplate(id,
                                           name,
                                           default_duration_in_minutes,
                                           color,
                                           loggedInUserId))
        {
            answerOk("could not update appointment template", false);
            return;
        }
        answerOk("appointment template updated", true);
        return;
    }
}
