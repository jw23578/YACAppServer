#ifndef TABLEFIELDS_H
#define TABLEFIELDS_H

#include <string>
#include <jw78Macros.h>

class TableFields
{
public:
    MACRO_ConstString(id);
    MACRO_ConstString(app_id);
    MACRO_ConstString(data);
    MACRO_ConstString(image_id);
    MACRO_ConstString(message_id);
    MACRO_ConstString(receiver_id);
    MACRO_ConstString(received_datetime);
    MACRO_ConstString(read_datetime);
    MACRO_ConstString(reader_id);
    MACRO_ConstString(fstname);
    MACRO_ConstString(surname);
    MACRO_ConstString(visible_name);
    MACRO_ConstString(task_start);
    MACRO_ConstString(task_end);
    MACRO_ConstString(user_id);
    MACRO_ConstString(task);
    MACRO_ConstString(device_token);

    TableFields() noexcept;
};

#endif // TABLEFIELDS_H
