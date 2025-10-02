#ifndef DEVICETOKENCACHE_H
#define DEVICETOKENCACHE_H

#include "cacheinterface.h"

class DeviceTokenCache: public CacheInterface
{
    std::map<reducedsole::uuid, std::set<std::string>> userId2DeviceToken;
public:
    DeviceTokenCache(DatabaseLogics &databaseLogics);

    void add(CurrentContext &context,
             const std::string &deviceToken);
    void remove(CurrentContext &context,
                const reducedsole::uuid &userId,
                const std::string &deviceToken);
    size_t get(CurrentContext &context,
               const reducedsole::uuid &userId,
               std::set<std::string> &deviceToken);
};

#endif // DEVICETOKENCACHE_H
