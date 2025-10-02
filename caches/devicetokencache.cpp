#include "devicetokencache.h"
#include "orm_implementions/t0015_user_devicetoken.h"

DeviceTokenCache::DeviceTokenCache(DatabaseLogics &databaseLogics):
    CacheInterface(databaseLogics)
{

}

void DeviceTokenCache::add(CurrentContext &context, const std::string &deviceToken)
{
    auto itUserId(userId2DeviceToken.find(context.userId));
    if (itUserId == userId2DeviceToken.end())
    {
        // unknown User, fetch all known DeviceToken and create User entry
        t0015_user_devicetoken userDeviceToken;
        userDeviceToken.fetchDeviceToken(context, context.userId, userId2DeviceToken[context.userId]);
        itUserId = userId2DeviceToken.find(context.userId);
    }
    auto itDeviceToken(itUserId->second.find(deviceToken));
    if (itDeviceToken != itUserId->second.end())
    {
        // DeviceToken already known
        return;
    }
    // insert DeviceToken in cache and save DeviceToken in Database
    itUserId->second.insert(deviceToken);

    t0015_user_devicetoken userDeviceToken;
    userDeviceToken.user_id = context.userId;
    userDeviceToken.device_token = deviceToken;
    userDeviceToken.store(context);
}

void DeviceTokenCache::remove(CurrentContext &context,
                              const reducedsole::uuid &userId,
                              const std::string &deviceToken)
{
    auto itUserId(userId2DeviceToken.find(userId));
    if (itUserId != userId2DeviceToken.end())
    {
        // user is known, try to remove deviceToken
        itUserId->second.erase(deviceToken);
    }
    t0015_user_devicetoken userDeviceToken;
    if (!userDeviceToken.load(context,
                             {{userDeviceToken.user_id.name(), userId.str()},
                              {userDeviceToken.device_token.name(), deviceToken}}))
    {
        return;
    }
    userDeviceToken.erase(context);
}

size_t DeviceTokenCache::get(CurrentContext &context,
                             const reducedsole::uuid &userId,
                             std::set<std::string> &deviceToken)
{
    auto itUserId(userId2DeviceToken.find(userId));
    if (itUserId != userId2DeviceToken.end())
    {
        // user is known, return all deviceToken
        deviceToken = itUserId->second;
        return deviceToken.size();
    }
    t0015_user_devicetoken userDeviceToken;
    if (userDeviceToken.fetchDeviceToken(context, userId, deviceToken))
    {
        userId2DeviceToken[userId] = deviceToken;
    }
    return deviceToken.size();
}
