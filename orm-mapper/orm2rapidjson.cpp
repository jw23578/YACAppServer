#include "orm2rapidjson.h"

ORM2rapidjson::ORM2rapidjson()
{

}

void ORM2rapidjson::toJson(const YACBaseObject &object,
                           rapidjson::Value &target,
                           rapidjson::MemoryPoolAllocator<> &alloc)
{
    target.SetObject();
    rapidjson::Value name("ORMName", alloc);
    rapidjson::Value ORMName(object.getORMName(), alloc);
    target.AddMember(name, ORMName, alloc);
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

YACBaseObject *ORM2rapidjson::fromJson(const rapidjson::Value &source,
                                       const ORMFactory &factory)
{
    if (!source.HasMember("ORMName"))
    {
        return 0;
    }
    YACBaseObject *object(factory.create(source["ORMName"].GetString()));
    if (!object)
    {
        return 0;
    }
    fromJson(source, *object);
    return object;
}

bool ORM2rapidjson::fromJson(const rapidjson::Value &source,
                             YACBaseObject &object)
{
    if (!source.HasMember("ORMName"))
    {
        return false;
    }
    if (object.getORMName() != source["ORMName"].GetString())
    {
        return false;
    }
    std::set<ORMString> propertyNames;
    object.getPropertyNames(propertyNames);
    for (const auto &pn : propertyNames)
    {
        if (!source.HasMember(pn) || source[pn].IsNull())
        {
            object.setPropertyNull(pn, true);
        }
        else
        {
            object.setPropertyFromString(pn, source[pn].GetString());
        }
    }
    return true;
}

void ORM2rapidjson::toJson(const std::set<YACBaseObject *> &objects,
                           rapidjson::Value &array,
                           rapidjson::MemoryPoolAllocator<> &alloc)
{
    array.SetArray();
    for (const auto &o: objects)
    {
        rapidjson::Value object;
        toJson(*o, object, alloc);
        array.PushBack(object, alloc);
    }
}

size_t ORM2rapidjson::fromJson(const rapidjson::Value &array,
                               const ORMFactory &factory,
                               std::set<YACBaseObject *> &objects)
{
    const auto it(array.Begin());
    while (it != array.End())
    {
        YACBaseObject *object(fromJson(*it, factory));
        if (object)
        {
            objects.insert(object);
        }
    }
    return objects.size();
}
