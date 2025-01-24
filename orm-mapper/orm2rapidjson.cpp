#include "orm2rapidjson.h"

ORM2rapidjson::ORM2rapidjson()
{

}

void ORM2rapidjson::toJson(const YACBaseObject &object, rapidjson::Document &document)
{
    toJson(object, document, document.GetAllocator());
}

void ORM2rapidjson::toJson(const YACBaseObject &object,
                           rapidjson::Value &target,
                           rapidjson::MemoryPoolAllocator<> &alloc)
{
    target.SetObject();
    ExtRapidJSONWriter writer(target, alloc);
    writer.addMember("ORMName", object.getORMName());
    for (const auto &p: object.getProperties())
    {
        add(writer, *p);
    }
}

YACBaseObject *ORM2rapidjson::fromJson(const rapidjson::Value &source,
                                            const YACORMFactory &factory)
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
    if (!source.IsObject())
    {
        return false;
    }
    if (source.ObjectEmpty())
    {
        return false;
    }
    if (!source.HasMember("ORMName"))
    {
        return false;
    }
    if (object.getORMName() != source["ORMName"].GetString())
    {
        return false;
    }
    for (const auto &pn : object.propertyNames())
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

void ORM2rapidjson::toJson(const std::set<YACBaseObject*> &objects,
                           rapidjson::Value &array,
                           rapidjson::MemoryPoolAllocator<> &alloc)
{
    array.SetArray();
    for (const auto &o: objects)
    {
        rapidjson::Value jsonObject;
        toJson(*o, jsonObject, alloc);
        array.PushBack(jsonObject, alloc);
    }
}

void ORM2rapidjson::addToArray(const YACBaseObject &object,
                               rapidjson::Value &array,
                               rapidjson::MemoryPoolAllocator<> &alloc)
{
    if (!array.IsArray())
    {
        array.SetArray();
    }
    rapidjson::Value jsonObject;
    toJson(object, jsonObject, alloc);
    array.PushBack(jsonObject, alloc);
}

size_t ORM2rapidjson::fromJson(const rapidjson::Value &array,
                               const YACORMFactory &factory,
                               std::set<YACBaseObject*> &objects)
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

void ORM2rapidjson::add(ExtRapidJSONWriter &writer, const ORMPropertyInterface &pi)
{
    if (pi.isNull())
    {
        writer.addNull(pi.name());
    }
    else
    {
        writer.addMember(pi.name(), pi.asString());
    }
}
