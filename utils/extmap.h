#ifndef EXTMAP_H
#define EXTMAP_H

#include <map>
#include <string>

#define MACRO_CreateDataMAP(value) \
    std::map<std::string, std::string> data; \
    data[#value] = value;

class ExtMap
{
public:
    ExtMap();
};

#endif // EXTMAP_H
