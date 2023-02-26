#include "handlerappuserupdateprofile.h"
#include "base64.h"
#include "definitions.h"

HandlerAppUserUpdateProfile::HandlerAppUserUpdateProfile(PistacheServerInterface &serverInterface,
                                                         DatabaseLogics &databaseLogics,
                                                         DeviceTokenCache &deviceTokenCache,
                                                         LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             methodNames.updateAppUserProfile,
                             TypePost,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics),
    deviceTokenCache(deviceTokenCache)
{
    addMethod(serverInterface,
              methodNames.updateDeviceToken,
              TypePost);
}

void HandlerAppUserUpdateProfile::method()
{
    if (isMethod(methodNames.updateDeviceToken))
    {
        MACRO_GetMandatoryString(deviceToken);
        deviceTokenCache.add(loggedInUserId,
                             deviceToken);
        answerOk("deviceToken stored",
                 true);
        return;
    }
    MACRO_GetMandatoryString(fstname);
    MACRO_GetMandatoryString(surname);
    MACRO_GetMandatoryString(visible_name);
    MACRO_GetMandatoryBool(searching_exactly_allowed);
    MACRO_GetMandatoryBool(searching_fuzzy_allowed);
    MACRO_GetBool(with_image);
    MACRO_GetString(image_data_base64);
    MACRO_GetString(public_key_base64);

    sole::uuid imageId(NullUuid);
    if (with_image)
    {
        if (image_data_base64.size() == 0)
        {
            answerOk("missing image data", false);
            return;
        }
        std::vector<char> data;
        bin_base64_decode(image_data_base64, data);
        std::basic_string<std::byte> image_data(reinterpret_cast<std::byte*>(data.data()), data.size());
        std::string message;
        if (!databaseLogics.databaseLogicImageTable.storeImage(image_data,
                                                               message,
                                                               imageId))
        {
            answerOk(message, false);
            return;
        }
    }       

    MACRO_GetString(deviceToken);
    if (deviceToken.size())
    {
        deviceTokenCache.add(loggedInUserId,
                             deviceToken);
    }

    std::string message;
    if (!databaseLogics.databaseLogicAppUser.updateAppUser(appId,
                                                           loggedInUserId,
                                                           fstname,
                                                           surname,
                                                           visible_name,
                                                           searching_exactly_allowed,
                                                           searching_fuzzy_allowed,
                                                           public_key_base64,
                                                           with_image,
                                                           imageId,
                                                           message))
    {
        answerOk(message, false);
        return;
    }
    answerOk(message, true);
}
