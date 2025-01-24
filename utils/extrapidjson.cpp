#include "extrapidjson.h"

ExtRapidJSON::ExtRapidJSON(const rapidjson::Value &document):
    document(document)
{

}

std::string ExtRapidJSON::getString(const std::string &name) const
{
    if (!document.HasMember(name.c_str()))
    {
        return "";
    }
    if (!document[name.c_str()].IsString())
    {
        return "";
    }
    return document[name.c_str()].GetString();
}

std::string ExtRapidJSON::getString(const std::string &section,
                                    const std::string &name) const
{
    if (!document.HasMember(section.c_str()))
    {
        return "";
    }
    rapidjson::Value const &sectionValue(document[section.c_str()]);
    if (!sectionValue.IsObject())
    {
        return "";
    }
    if (!sectionValue.HasMember(name.c_str()))
    {
        return "";
    }
    if (!sectionValue[name.c_str()].IsString())
    {
        return "";
    }
    return sectionValue[name.c_str()].GetString();
}

int ExtRapidJSON::getInt(const std::string &name) const
{
    if (!document.HasMember(name.c_str()))
    {
        return 0;
    }
    if (!document[name.c_str()].IsInt())
    {
        return 0;
    }
    return document[name.c_str()].GetInt();
}

int ExtRapidJSON::getInt(const std::string &section,
                         const std::string &name) const
{
    if (!document.HasMember(section.c_str()))
    {
        return 0;
    }
    rapidjson::Value const &sectionValue(document[section.c_str()]);
    if (!sectionValue.IsObject())
    {
        return 0;
    }
    if (!sectionValue.HasMember(name.c_str()))
    {
        return 0;
    }
    if (!sectionValue[name.c_str()].IsInt())
    {
        return 0;
    }
    return sectionValue[name.c_str()].GetInt();
}

bool ExtRapidJSON::getBool(const std::string &name) const
{
    if (!document.HasMember(name.c_str()))
    {
        return false;
    }
    return document[name.c_str()].GetBool();
}

bool ExtRapidJSON::hasValue(std::string const &name) const
{
    return document.HasMember(name.c_str());
}

ExtRapidJSONWriter::ExtRapidJSONWriter(rapidjson::Value &objectValue,
                                       rapidjson::Document::AllocatorType &allocator):
    objectValue(objectValue),
    allocator(allocator)
{

}

void ExtRapidJSONWriter::addNull(const std::string &name)
{
    objectValue.AddMember(rapidjson::Value(name, allocator).Move(),
                          rapidjson::Value(rapidjson::Type::kNullType),
                          allocator);
}

void ExtRapidJSONWriter::addMember(const std::string &name, const char * const value)
{
    addMember(name, std::string(value));
}

void ExtRapidJSONWriter::addMember(const std::string &name, const std::string &value)
{
    objectValue.AddMember(rapidjson::Value(name, allocator).Move(),
                          rapidjson::Value(value, allocator).Move(),
                          allocator);
}

void ExtRapidJSONWriter::addMember(const std::string &name, const int64_t value)
{
    objectValue.AddMember(rapidjson::Value(name, allocator).Move(),
                          value,
                          allocator);
}

void ExtRapidJSONWriter::addMember(const std::string &name, const unsigned int value)
{
    objectValue.AddMember(rapidjson::Value(name, allocator).Move(),
                          value,
                          allocator);
}

void ExtRapidJSONWriter::addMember(const std::string &name, const int value)
{
    objectValue.AddMember(rapidjson::Value(name, allocator).Move(),
                          value,
                          allocator);
}

void ExtRapidJSONWriter::addMember(const std::string &name, const bool value)
{
    objectValue.AddMember(rapidjson::Value(name, allocator).Move(),
                          value,
                          allocator);
}

