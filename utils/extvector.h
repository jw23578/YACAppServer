#ifndef EXTVECTOR_H
#define EXTVECTOR_H

#include <string>
#include <vector>

class ExtVector
{
public:
    ExtVector();
    static std::string concat(const std::vector<std::string> &v,
                              const std::string &delim,
                              const std::string &prefix,
                              const std::string &suffix);
};

#endif // EXTVECTOR_H
