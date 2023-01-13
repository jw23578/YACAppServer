#ifndef TABLENAMES_H
#define TABLENAMES_H

#include <string>
#include <jw78Macros.h>

class TableNames
{
public:
    MACRO_ConstString(t0001_users);
    MACRO_ConstString(t0002_apps);
    MACRO_ConstString(t0003_appuser_profiles);
    MACRO_ConstString(t0009_appuser_logintoken);
    MACRO_ConstString(t0004_appuser_passwordhashes);
    MACRO_ConstString(t0005_group_of_appusers);
    MACRO_ConstString(t0006_appuser2group);
    MACRO_ConstString(t0007_messages);
    MACRO_ConstString(t0008_message_received);
    MACRO_ConstString(t0010_task);
    MACRO_ConstString(t0011_task_time);
    MACRO_ConstString(t0012_worktime);
    MACRO_ConstString(t0013_images);
    MACRO_ConstString(t0014_message_read);
    MACRO_ConstString(t0015_appuser_devicetoken);

    TableNames();
};

#endif // TABLENAMES_H
