#ifndef TABLENAMES_H
#define TABLENAMES_H

#include <string>

class TableNames
{
public:
    const std::string t0001_users = "t0001_users";
    const std::string t0002_apps = {"t0002_apps"};
    const std::string t0003_appuser_profiles = {"t0003_appuser_profiles"};
    const std::string t0009_appuser_logintoken = {"t0009_appuser_logintoken"};
    const std::string t0004_appuser_passwordhashes = {"t0004_appuser_passwordhashes"};
    const std::string t0005_group_of_appusers = {"t0005_group_of_appusers"};
    const std::string t0006_appuser2group = {"t0006_appuser2group"};
    const std::string t0007_messages = {"t0007_messages"};
    const std::string t0008_message_state {"t0008_message_state"};
    const std::string t0010_task {"t0010_task"};
    const std::string t0011_task_time {"t0011_task_time"};
    const std::string t0012_worktime {"t0012_worktime"};

    TableNames();
};

#endif // TABLENAMES_H
