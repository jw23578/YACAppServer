#ifndef JW78FIREBASEWRAPPER_H
#define JW78FIREBASEWRAPPER_H

#include <string>

namespace jw78
{

class FirebaseWrapper
{
public:
    FirebaseWrapper();

    bool legacySend(const std::string &key,
                    const std::string &payload,
                    std::string &result);
};

}

#endif // JW78FIREBASEWRAPPER_H
