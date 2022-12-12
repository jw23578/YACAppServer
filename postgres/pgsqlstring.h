#ifndef JW78CORE_SQL_STRING_H
#define JW78CORE_SQL_STRING_H

#include <string>
#include <chrono>
#include <iconv.h>
#include <vector>
#include <map>
#include "sole/sole.hpp"
#include "extstring.h" // marked as unused but is needed for MACRO_set

#define MACRO_set(fieldValue) \
    sql.set(ExtString::lower(#fieldValue), fieldValue);

#define MACRO_setId() \
    sql.set("id", sole::uuid4());


class PGSqlString
{
    std::string sql;
    bool firstSetField;
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
    void update(std::string const &tableName);

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
    void addSet(const std::string &needle,
                const T &value)
    {
        addCompare(firstSetField ? "" : ", ",
                   needle,
                   "=",
                   value);
        firstSetField = false;
    }
};

#endif // JW78CORE_SQL_STRING_H
