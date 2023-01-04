#ifndef TABLEFIELDS_H
#define TABLEFIELDS_H

#include <string>

class TableFields
{
public:
    const std::string id = {"id"};
    const std::string app_id = {"app_id"};
    const std::string data = {"data"};
    const std::string image_id = {"image_id"};
    const std::string message_id = {"message_id"};
    const std::string receiver_id = {"receiver_id"};
    const std::string received_datetime = {"received_datetime"};
    const std::string read_datetime = {"read_datetime"};
    const std::string reader_id = {"reader_id"};
    const std::string fstname = {"fstname"};
    const std::string surname = {"surname"};
    const std::string visible_name = {"visible_name"};
    const std::string task_start = {"task_start"};
    const std::string task_end = {"task_end"};
    const std::string user_id = {"user_id"};
    const std::string task = {"task"};

    TableFields();
};

#endif // TABLEFIELDS_H
