#include "pgsqlstring.h"
#include <algorithm>
#include "utils/extstring.h"
#include "utils/definitions.h"

PGSqlString::PGSqlString():
    firstSetField(true)
{
}

PGSqlString::PGSqlString(std::string const &s):
    sql(s),
    firstSetField(true)
{

}

void PGSqlString::update(const std::string &tableName)
{
    firstSetField = true;
    sql = "update " + tableName + " set ";
}

void PGSqlString::insert(const std::string &tableName)
{
    firstSetField = true;
    sql = "insert into " + tableName + " ( ";
}

void PGSqlString::addOnConflict(const std::string &target,
                                PGSqlString &onConflict)
{
    conflictAction = " on conflict (";
    conflictAction += target;
    conflictAction += " ) ";
    conflictAction += onConflict.str();
}

void PGSqlString::rawReplace(std::string &sql,
                             std::string const &param,
                             std::string value) const
{
    std::string p(":");
    p += param;
    ExtString::replaceAll(sql, p, value);
    p = std::string(": ") + param;
    ExtString::replaceAll(sql, p, value);
}

std::string PGSqlString::replaceVariables() const
{
    std::string copy(sql);
    if (insert2ndPart.size())
    {
        copy += ") values (";
        copy += insert2ndPart;
        copy += ") ";
        if (conflictAction.size())
        {
            copy += conflictAction;
        }
    }
    std::vector<std::string> variableNames;
    variableNames.reserve(variable2Values.size());
    for (auto const &v : variable2Values)
    {
        variableNames.push_back(v.first);
    }
    std::sort(variableNames.begin(), variableNames.end(), [](std::string const &a, std::string const &b) {return a > b; });
    for (auto const &n : variableNames)
    {
        rawReplace(copy, n, variable2Values.at(n));
    }
    return copy;
}

std::string PGSqlString::quote(std::string const &s)
{
    return std::string("'") + s + std::string("'");
}


std::string PGSqlString::esc(std::string const &s)
{
    return ExtString::escape(s, "\'", "\'");
}



void PGSqlString::set(std::string const &param,
                      std::string const &value)
{
    variable2Values[param] = quote(esc(value));
}

void PGSqlString::set(const std::string &param,
                      const sole::uuid &value)
{
    if (value == NullUuid)
    {
        variable2Values[param] = "null";
        return;
    }
    variable2Values[param] = quote(value.str());
}

void PGSqlString::set(std::string const &param,
                      const char * const value)
{
    variable2Values[param] = quote(esc(value));
}
void PGSqlString::set(std::string const &param,
                      uint64_t value)
{
    variable2Values[param] = ExtString::toString(value);
}

void PGSqlString::set(std::string const &param,
                      int value)
{
    variable2Values[param] = ExtString::toString(value);
}

void PGSqlString::set(const std::string &param,
                      unsigned int value)
{
    variable2Values[param] = ExtString::toString(value);
}

void PGSqlString::set(std::string const &param,
                      double value)
{
    variable2Values[param] = ExtString::toString(value);
}

void PGSqlString::set(const std::string &param,
                      bool value)
{
    variable2Values[param] = value ? "true" : "false";
}
void PGSqlString::set(std::string const &param,
                      std::chrono::system_clock::time_point const &tp)
{
    if (tp == TimePointPostgreSqlNull)
    {
        variable2Values[param] = "null";
        return;
    }
    if (tp == TimePointPostgreSqlNow)
    {
        variable2Values[param] = "now()";
        return;
    }
    std::string v("to_timestamp(");
    int64_t milliseconds(std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count());
    v += ExtString::toString(milliseconds / 1000.0);
    v += ") ";
    variable2Values[param] = v;
}

void PGSqlString::set(const std::string &param,
                      const std::vector<uint64_t> &v)
{
    if (v.size() == 0)
    {
        variable2Values[param] = "null";
        return;
    }
    std::stringstream value;
    value << v[0];
    for (size_t i(1); i < v.size(); ++i)
    {
        value << ", ";
        value << v[i];
    }
    variable2Values[param] = value.str();
}

void PGSqlString::set(const std::string &param,
                      const std::set<sole::uuid> &uuids)
{
    if (uuids.size() == 0)
    {
        variable2Values[param] = "null";
        return;
    }
    std::stringstream value;
    bool first(true);
    for (const auto &u : uuids)
    {
        if (!first)
        {
            value << ", ";
        }
        first = false;
        value << quote(u.str());
    }
    variable2Values[param] = value.str();
}

PGSqlString &PGSqlString::operator=(std::string const &s)
{
    sql = s;
    return *this;
}

PGSqlString &PGSqlString::operator+=(std::string const &s)
{
    sql += s;
    return *this;
}

char &PGSqlString::operator[](size_t i)
{
    return sql[i];
}

PGSqlString &PGSqlString::operator+=(const PGSqlString &s)
{
    sql += s.sql;
    return *this;
}


std::string PGSqlString::str() const
{
    return replaceVariables();
}

size_t PGSqlString::size() const
{
    return sql.size();
}

void PGSqlString::replace(const std::string &needle,
                          const std::string &value)
{
    ExtString::replaceAll(sql, needle, value);
}

void PGSqlString::limit(const size_t limit, const size_t offset)
{
    sql += " limit " + ExtString::toString(limit);
    sql += " offset " + ExtString::toString(offset);
}
