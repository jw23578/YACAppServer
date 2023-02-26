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
    MACRO_GetMandatoryUuid(id);
    if (isMethod(methodNames.deleteAppointment))
    {
        std::string message("appointment deleted");
        answerOk(message, dla.deleteAppointment(id, loggedInUserId, message));
        return;
    }
    if (isMethod(methodNames.insertAppointment))
    {
        MACRO_GetMandatoryUuid(appointment_group_id);
        MACRO_GetMandatoryUuid(appointment_template_id);
        MACRO_GetMandatoryString(caption);
        MACRO_GetMandatoryString(decription);
        MACRO_GetMandatoryTimePointFromISO(start_datetime);
        MACRO_GetMandatoryTimePointFromISO(end_datetime);
        MACRO_GetMandatoryInt(max_bookable_count, true);
        MACRO_GetMandatoryTimePointFromISO(bookable_since_datetime);
        MACRO_GetMandatoryTimePointFromISO(bookable_until_datetime);
        MACRO_GetMandatoryInt(booking_credits, true);

        std::string message("appointment inserted");
        answerOk(message, dla.insertAppointment(id,
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
                                                message));
        return;
    }
    if (isMethod(methodNames.deleteAppointmentTemplate))
    {
        std::string message("appointmentTemplate deleted");
        answerOk(message, dla.deleteAppointmentTemplate(id, loggedInUserId, message));
        return;
    }
    MACRO_GetMandatoryString(name);
    MACRO_GetMandatoryInt(default_duration_in_minutes, false);
    MACRO_GetMandatoryInt(color, true);
    if (isMethod(methodNames.insertAppointmentTemplate))
    {
        if (!dla.insertAppointmentTemplate(id,
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
