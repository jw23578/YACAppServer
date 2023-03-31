#include "orm2rapidjson.h"

ORM2rapidjson::ORM2rapidjson()
{

}

void ORM2rapidjson::toJson(ORMObjectInterface &object,
                           rapidjson::Value &target,
                           rapidjson::MemoryPoolAllocator<> &alloc)
{
    target.SetObject();
    std::set<ORMString> propertyNames;
    object.getPropertyNames(propertyNames);
    for (const auto &pn : propertyNames)
    {
        rapidjson::Value name(pn, alloc);
        if (object.propertyIsNull(pn))
        {
            target.AddMember(name, rapidjson::Value(rapidjson::Type::kNullType) , alloc);
        }
        else
        {
            rapidjson::Value value(object.getPropertyToString(pn), alloc);
            target.AddMember(name, value, alloc);
        }
    }
}

void ORM2rapidjson::fromJson(rapidjson::Value &source,
                             ORMObjectInterface &object)
{
    std::set<ORMString> propertyNames;
    object.getPropertyNames(propertyNames);
    for (const auto &pn : propertyNames)
    {
        source.HasMember(pn);

        if (!source.HasMember(pn) || source[pn].IsNull())
        {
            object.setPropertyNull(pn, true);
        }
        else
        {
            object.setPropertyFromString(pn, source[pn].GetString());
        }
    }
}
