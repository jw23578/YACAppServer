#ifndef ORM2RAPIDJSON_H
#define ORM2RAPIDJSON_H

#include "rapidjson/document.h"
#include "orm_implementions/yacbaseobject.h"
#include "orm_implementions/yacormfactory.h"

class ORM2rapidjson
{
public:
    ORM2rapidjson();

    void toJson(const ORMObjectInterface &object,
                rapidjson::Value &target,
                rapidjson::MemoryPoolAllocator<> &alloc);

    ORMObjectInterface *fromJson(const rapidjson::Value &source,
                                 const YACORMFactory &factory);

    bool fromJson(const rapidjson::Value &source,
                  ORMObjectInterface &object);

    void toJson(const std::set<ORMObjectInterface*> &objects,
                rapidjson::Value &array,
                rapidjson::MemoryPoolAllocator<> &alloc);

    void addToArray(const ORMObjectInterface &object,
                    rapidjson::Value &array,
                    rapidjson::MemoryPoolAllocator<> &alloc);

    size_t fromJson(const rapidjson::Value &array,
                    const YACORMFactory &factory,
                    std::set<ORMObjectInterface *> &objects);
};

#endif // ORM2RAPIDJSON_H
