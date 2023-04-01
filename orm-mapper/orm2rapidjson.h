#ifndef ORM2RAPIDJSON_H
#define ORM2RAPIDJSON_H

#include "rapidjson/document.h"
#include "orm_implementions/yacbaseobject.h"
#include "orm_implementions/ormfactory.h"

class ORM2rapidjson
{
public:
    ORM2rapidjson();

    void toJson(const YACBaseObject &object,
                rapidjson::Value &target,
                rapidjson::MemoryPoolAllocator<> &alloc);

    YACBaseObject *fromJson(const rapidjson::Value &source,
                            const ORMFactory &factory);

    bool fromJson(const rapidjson::Value &source,
                  YACBaseObject &object);

    void toJson(const std::set<YACBaseObject *> &objects,
                rapidjson::Value &array,
                rapidjson::MemoryPoolAllocator<> &alloc);

    size_t fromJson(const rapidjson::Value &array,
                    const ORMFactory &factory,
                    std::set<YACBaseObject*> &objects);
};

#endif // ORM2RAPIDJSON_H
