#ifndef PGCOLUMNANDTYPE_H
#define PGCOLUMNANDTYPE_H

#include <string>
#include <map>

enum PGTypes
{
    pg_int,
    pg_bigint,
    pg_text,
    pg_float,
    pg_bool,
    pg_timestamp,
    pg_uuid
};

class PGColumnAndType
{
    static const std::map<PGTypes, std::string> typeToString;
  public:
    const std::string column;
    const PGTypes type;
    const bool primaryKey;
    const bool index;
    PGColumnAndType(const std::string &column, PGTypes type);
    PGColumnAndType(const std::string &column, PGTypes type, const bool primaryKey);
    PGColumnAndType(const std::string &column, PGTypes type, const bool primaryKey, const bool index);
    std::string typeString() const;
    std::string toString() const;
    std::string primaryKeyString() const;
};

#endif // PGCOLUMNANDTYPE_H
