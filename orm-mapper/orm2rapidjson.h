#ifndef ORM2RAPIDJSON_H
#define ORM2RAPIDJSON_H

#include "rapidjson/document.h"
#include "orm/ormobjectinterface.h"
#include "orm_implementions/ormfactory.h"

class ORM2rapidjson
{
public:
    ORM2rapidjson();

    void toJson(const ORMObjectInterface &object,
                rapidjson::Value &target,
                rapidjson::MemoryPoolAllocator<> &alloc);

    ORMObjectInterface *fromJson(const rapidjson::Value &source,
                                 const ORMFactory &factory);

    bool fromJson(const rapidjson::Value &source,
                  ORMObjectInterface &object);

    void toJson(const std::set<ORMObjectInterface*> &objects,
                rapidjson::Value &array,
                rapidjson::MemoryPoolAllocator<> &alloc);

    size_t fromJson(const rapidjson::Value &array,
                    const ORMFactory &factory,
                    std::set<ORMObjectInterface*> &objects);
};

#endif // ORM2RAPIDJSON_H
