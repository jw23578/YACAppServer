#ifndef DEVICETOKENCACHE_H
#define DEVICETOKENCACHE_H

#include "cacheinterface.h"

class DeviceTokenCache: public CacheInterface
{
    std::map<reducedsole::uuid, std::set<std::string>> userId2DeviceToken;
public:
    DeviceTokenCache(DatabaseLogics &databaseLogics);

    void add(const reducedsole::uuid &userId,
             const std::string &deviceToken);
    void remove(const reducedsole::uuid &userId,
                const std::string &deviceToken);
    size_t get(const reducedsole::uuid &userId,
               std::set<std::string> &deviceToken);
};

#endif // DEVICETOKENCACHE_H
