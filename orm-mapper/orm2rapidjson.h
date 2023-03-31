#ifndef ORM2RAPIDJSON_H
#define ORM2RAPIDJSON_H

#include "rapidjson/document.h"
#include "orm/ormobjectinterface.h"

class ORM2rapidjson
{
public:
    ORM2rapidjson();

    void toJson(ORMObjectInterface &object,
                rapidjson::Value &target,
                rapidjson::MemoryPoolAllocator<> &alloc);

    void fromJson(rapidjson::Value &source,
                  ORMObjectInterface &object);
};

#endif // ORM2RAPIDJSON_H
