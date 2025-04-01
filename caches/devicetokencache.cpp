#include "devicetokencache.h"

DeviceTokenCache::DeviceTokenCache(DatabaseLogics &databaseLogics):
    CacheInterface(databaseLogics)
{

}

void DeviceTokenCache::add(const reducedsole::uuid &userId, const std::string &deviceToken)
{
    auto itUserId(userId2DeviceToken.find(userId));
    if (itUserId == userId2DeviceToken.end())
    {
        // unknown User, fetch all known DeviceToken and create User entry
        databaseLogics.databaseLogicAppUser.fetchDeviceToken(userId,
                                                             userId2DeviceToken[userId]);
        itUserId = userId2DeviceToken.find(userId);
    }
    auto itDeviceToken(itUserId->second.find(deviceToken));
    if (itDeviceToken != itUserId->second.end())
    {
        // DeviceToken already known
        return;
    }
    // insert DeviceToken in cache and save DeviceToken in Database
    itUserId->second.insert(deviceToken);
    databaseLogics.databaseLogicAppUser.storeDeviceToken(userId,
                                                         deviceToken);
}

void DeviceTokenCache::remove(const reducedsole::uuid &userId,
                              const std::string &deviceToken)
{
    auto itUserId(userId2DeviceToken.find(userId));
    if (itUserId != userId2DeviceToken.end())
    {
        // user is known, try to remove deviceToken
        itUserId->second.erase(deviceToken);
    }
    // in either way remove from the database
    databaseLogics.databaseLogicAppUser.removeDeviceToken(userId,
                                                          deviceToken);
}

size_t DeviceTokenCache::get(const reducedsole::uuid &userId,
                             std::set<std::string> &deviceToken)
{
    auto itUserId(userId2DeviceToken.find(userId));
    if (itUserId != userId2DeviceToken.end())
    {
        // user is known, return all deviceToken
        deviceToken = itUserId->second;
        return deviceToken.size();
    }
    if (databaseLogics.databaseLogicAppUser.fetchDeviceToken(userId,
                                                             deviceToken))
    {
        userId2DeviceToken[userId] = deviceToken;
    }
    return deviceToken.size();
}
