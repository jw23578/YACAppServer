#include "methodinfo.h"

MethodInfo::MethodInfo(const MethodType type,
                       const std::string &method,
                       const std::string &description):
    type(type),
    method(method),
    description(description)
{}

const std::string &MethodInfo::getMethod() const
{
    return method;
}

const std::string MethodInfo::getTypeAsString() const
{
    switch (type)
    {
    case TypeGet: return "get";
    case TypePost: return "post";
    case TypeDelete: return "delete";
    case TypePut: return "put";
    }
    return "";
}
