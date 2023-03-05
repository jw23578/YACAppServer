#ifndef JW78CORE_SQL_STRING_H
#define JW78CORE_SQL_STRING_H

#include <string>
#include <chrono>
#include <iconv.h>
#include <vector>
#include <map>
#include "sole/sole.hpp"
#include "extstring.h" // marked as unused but is needed for MACRO_set
#include "utils/definitions.h"

#define MACRO_set(sql, fieldValue) \
    sql.set(ExtString::lower(#fieldValue), fieldValue);

#define MACRO_setId() \
    sql.set("id", sole::uuid4());

#define MACRO_addInsert(sql, fieldValue) \
    sql.addInsert(#fieldValue, fieldValue);

#define MACRO_addSet(sql, fieldValue) \
    sql.addSet(#fieldValue, fieldValue);

#define MACRO_addInsertOrSet(sql, fieldValue) \
    sql.addInsertOrSet(#fieldValue, fieldValue);


class PGSqlString
{
    std::string sql;
    std::string insert2ndPart;
    std::string conflictAction;
    bool firstSetField{true};
    bool selectStatement{false};
    bool insertStatement{false};
    bool updateStatement{false};
    bool deleteStatement{false};
    void rawReplace(std::string &sql,
                    std::string const &param,
                    std::string value) const;
    std::string replaceVariables() const;
    std::string esc(std::string const &s);
    std::string quote(std::string const &s);
    std::map<std::string, std::string> variable2Values;
public:
    PGSqlString();
    PGSqlString(std::string const &s);
    void select(std::string const &tableName);
    void update(std::string const &tableName);
    void insert(std::string const &tableName);
    void delet(std::string const &tableName);
    void insertOrUpdate(sole::uuid &id, std::string const &tableName);

    void addOnConflict(const std::string &target, PGSqlString &onConflict);

    void set(std::string const &param,
             std::string const &value);
    void set(std::string const &param,
             sole::uuid const &value);
    void set(std::string const &param,
             const char * const value);
    void set(std::string const &param,
             uint64_t value);
    void set(std::string const &param,
             int value);
    void set(std::string const &param,
             unsigned int value);
    void set(std::string const &param,
             double value);
    void set(std::string const &param,
             bool value);
    void set(std::string const &param,
             std::chrono::system_clock::time_point const &tp);
    void set(std::string const &param,
             std::vector<uint64_t> const &v);
    void set(const std::string &param,
             const std::set<sole::uuid> &uuids);
    PGSqlString &operator=(std::string const &s);
    PGSqlString &operator+=(std::string const &s);
    PGSqlString &operator+=(PGSqlString const &s);
    char &operator[](size_t i);
    std::string str() const;
    size_t size() const;

    void replace(const std::string &needle,
                 const std::string &value);

    template <class T>
    void addCompare(const std::string &connector,
                    const std::string &needle,
                    const std::string &comparator,
                    const T &value)
    {
        sql += " " + connector;
        sql += " " + needle;
        sql += " " + comparator;
        sql += " :" + needle;
        set(needle, value);
    }

    template <class T>
    void addSet(const std::string &field,
                const T &value)
    {
        addCompare(firstSetField ? "" : ", ",
                   field,
                   "=",
                   value);
        firstSetField = false;
    }

    template <class T>
    void addInsert(const std::string &field,
                   const T &value)
    {
        if (!firstSetField)
        {
            sql += ", ";
            insert2ndPart += ", ";
        }
        firstSetField = false;
        sql += field;
        insert2ndPart += ":" + field;
        set(field, value);
    }

    template <class T>
    void addInsertOrSet(const std::string &field, const T &value)
    {
        if (insertStatement)
        {
            addInsert(field, value);
        }
        if (updateStatement)
        {
            addSet(field, value);
        }
    }

    template <class T>
    void addInsertOrWhere(const std::string &connector,
                          const std::string &needleField,
                          const std::string &comparator,
                          const T &value)
    {
        if (insertStatement)
        {
            addInsert(needleField, value);
        }
        if (updateStatement)
        {
            addCompare(connector, needleField, comparator, value);
        }
    }

    void limit(const size_t limit, const size_t offset);
};

#endif // JW78CORE_SQL_STRING_H
