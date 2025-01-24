#ifndef EXTRAPIDJSON_H
#define EXTRAPIDJSON_H

#include "rapidjson/document.h"
#include <string>

class ExtRapidJSON
{
    rapidjson::Value const &document;
public:
    ExtRapidJSON(rapidjson::Value const &document);

    std::string getString(std::string const &name) const;
    std::string getString(std::string const &section,
                          std::string const &name) const;
    int getInt(std::string const &name) const;
    int getInt(std::string const &section,
               std::string const &name) const;
    bool getBool(std::string const &name) const;

    bool hasValue(std::string const &name) const;
};

class ExtRapidJSONWriter
{
    rapidjson::Value &objectValue;
    rapidjson::Document::AllocatorType &allocator;
public:
    ExtRapidJSONWriter(rapidjson::Value &objectValue,
                       rapidjson::Document::AllocatorType &allocator);

    void addNull(std::string const &name);
    void addMember(std::string const &name, const char * const value);
    void addMember(std::string const &name, std::string const &value);
    void addMember(std::string const &name, const int64_t value);
    void addMember(std::string const &name, const unsigned int value);
    void addMember(std::string const &name, const int value);
    void addMember(std::string const &name, const bool value);
};

#endif // EXTRAPIDJSON_H
