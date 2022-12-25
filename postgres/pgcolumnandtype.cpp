#include "pgcolumnandtype.h"

const std::map<PGTypes, std::string> PGColumnAndType::typeToString = {{pg_int, "int"},
                                                                      {pg_bigint, "bigint"},
                                                                      {pg_text, "text"},
                                                                      {pg_float, "float"},
                                                                      {pg_bool, "bool"},
                                                                      {pg_timestamp, "timestamp with time zone"},
                                                                      {pg_uuid, "uuid"},
                                                                      {pg_blob, "oid"}};


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

