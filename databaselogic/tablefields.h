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
    MACRO_ConstString(public_key_base64);
    MACRO_ConstString(deleted);
    MACRO_ConstString(name);
    MACRO_ConstString(appuser_id);
    MACRO_ConstString(password_hash);
    MACRO_ConstString(user_mood);
    MACRO_ConstString(day_rating);
    MACRO_ConstString(ts);
    MACRO_ConstString(type);
    MACRO_ConstString(creater_id);
    MACRO_ConstString(created);
    MACRO_ConstString(finished);
    MACRO_ConstString(verify_token);
    MACRO_ConstString(verify_token_valid_until);
    MACRO_ConstString(loginemail);
    MACRO_ConstString(verified);

    TableFields() noexcept;
};

#endif // TABLEFIELDS_H
