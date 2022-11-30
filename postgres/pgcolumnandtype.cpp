#include "pgcolumnandtype.h"

PGColumnAndType::PGColumnAndType(const std::string &column, PGTypes type):
    column(column),
    type(type),
    primaryKey(false),
    index(false)
{

}

PGColumnAndType::PGColumnAndType(const std::string &column, PGTypes type, const bool primaryKey):
    column(column),
    type(type),
    primaryKey(primaryKey),
    index(false)
{

}

PGColumnAndType::PGColumnAndType(const std::string &column, PGTypes type, const bool primaryKey, const bool index):
    column(column), type(type), primaryKey(primaryKey), index(index)
{

}

std::string PGColumnAndType::typeString() const
{
    return typeToString.at(type);
}

std::string PGColumnAndType::toString() const
{
    return column + " " + typeString();
}

std::string PGColumnAndType::primaryKeyString() const
{
    if (!primaryKey)
    {
        return "";
    }
    return std::string("primary key (") + column + ")";
}

