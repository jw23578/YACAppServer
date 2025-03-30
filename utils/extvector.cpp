#include "extvector.h"

ExtVector::ExtVector()
{

}

std::string ExtVector::concat(const std::vector<std::string> &v,
                              const std::string &delim,
                              const std::string &prefix,
                              const std::string &suffix,
                              const std::string &ifEmpty)
{
    if (v.empty())
    {
        return ifEmpty;
    }
    std::string ret(prefix);
    auto it(v.cbegin());
    if (it != v.cend())
    {
        ret += *it;
        ++it;
    }
    while (it != v.cend())
    {
        ret += delim;
        ret += *it;
        ++it;
    }
    return ret + suffix;
}
