#ifndef JW78CORE_SQL_STRING_H
#define JW78CORE_SQL_STRING_H

#include <string>
#include <chrono>
#include <iconv.h>
#include <vector>
#include <map>
#include "sole/sole.hpp"

class PGSqlString
{
    std::string sql;
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
    PGSqlString &operator=(std::string const &s);
    PGSqlString &operator+=(std::string const &s);
    PGSqlString &operator+=(PGSqlString const &s);
    char &operator[](size_t i);
    std::string str() const;
    size_t size() const;
};

#endif // JW78CORE_SQL_STRING_H