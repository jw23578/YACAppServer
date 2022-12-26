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

    TableFields();
};

#endif // TABLEFIELDS_H
