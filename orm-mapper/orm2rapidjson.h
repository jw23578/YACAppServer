#ifndef ORM2RAPIDJSON_H
#define ORM2RAPIDJSON_H

#include "rapidjson/document.h"
#include "orm_implementions/yacbaseobject.h"
#include "orm_implementions/yacormfactory.h"
#include "utils/extrapidjson.h"

class ORM2rapidjson
{
public:
    ORM2rapidjson();

    void toJson(const YACBaseObject &object,
                rapidjson::Document &document);
    void toJson(const YACBaseObject &object,
                rapidjson::Value &target,
                rapidjson::MemoryPoolAllocator<> &alloc);

    YACBaseObject *fromJson(const rapidjson::Value &source,
                            const YACORMFactory &factory);

    bool fromJson(const rapidjson::Value &source,
                  YACBaseObject &object);

    void toJson(const std::set<YACBaseObject*> &objects,
                rapidjson::Value &array,
                rapidjson::MemoryPoolAllocator<> &alloc);

    void addToArray(const YACBaseObject &object,
                    rapidjson::Value &array,
                    rapidjson::MemoryPoolAllocator<> &alloc);

    size_t fromJson(const rapidjson::Value &array,
                    const YACORMFactory &factory,
                    std::set<YACBaseObject*> &objects);

    void add(ExtRapidJSONWriter &writer, const ORMPropertyInterface &pi);
};

#endif // ORM2RAPIDJSON_H
