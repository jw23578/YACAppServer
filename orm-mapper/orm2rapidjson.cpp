#include "orm2rapidjson.h"

ORM2rapidjson::ORM2rapidjson()
{

}

void ORM2rapidjson::toJson(const ORMObjectInterface &object,
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

ORMObjectInterface *ORM2rapidjson::fromJson(const rapidjson::Value &source,
                                            const ORMFactory &factory)
{
    if (!source.HasMember("ORMName"))
    {
        return 0;
    }
    ORMObjectInterface *object(factory.create(source["ORMName"].GetString()));
    if (!object)
    {
        return 0;
    }
    fromJson(source, *object);
    return object;
}

bool ORM2rapidjson::fromJson(const rapidjson::Value &source,
                             ORMObjectInterface &object)
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

void ORM2rapidjson::toJson(const std::set<ORMObjectInterface *> &objects,
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
                               std::set<ORMObjectInterface *> &objects)
{
    const auto it(array.Begin());
    while (it != array.End())
    {
        ORMObjectInterface *object(fromJson(*it, factory));
        if (object)
        {
            objects.insert(object);
        }
    }
    return objects.size();
}
