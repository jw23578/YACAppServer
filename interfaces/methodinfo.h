#ifndef METHODINFO_H
#define METHODINFO_H

#include <string>

class MethodInfo
{
public:
    enum MethodType
    {
        TypeGet,
        TypePost,
        TypeDelete,
        TypePut
    };
private:
    MethodType type;
    std::string method;
    std::string description;
public:
    MethodInfo(const MethodType type,
               const std::string &method,
               const std::string &description);
    const std::string &getMethod() const;
    const std::string getTypeAsString() const;
};

#endif // METHODINFO_H
