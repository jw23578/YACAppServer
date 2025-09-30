#include "handlerappuserupdateprofile.h"
#include "base64.h"
#include "JWUtils/definitions.h"

HandlerAppUserUpdateProfile::HandlerAppUserUpdateProfile(PistacheServerInterface &serverInterface,
                                                         DatabaseLogics &databaseLogics,
                                                         DeviceTokenCache &deviceTokenCache,
                                                         LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
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

void HandlerAppUserUpdateProfile::method(CurrentContext &context)
{
    if (isMethod(methodNames.updateDeviceToken))
    {
        MACRO_GetMandatoryString(deviceToken);
        deviceTokenCache.add(context.userId,
                             deviceToken);
        answerOk("deviceToken stored",
                 true);
        return;
    }
    MACRO_GetMandatoryString(fstname);
    MACRO_GetMandatoryString(surname);
    MACRO_GetMandatoryString(visible_name);
    MACRO_GetMandatoryString(color);
    MACRO_GetMandatoryString(message_font_color);
    MACRO_GetMandatoryBool(searching_exactly_allowed);
    MACRO_GetMandatoryBool(searching_fuzzy_allowed);
    MACRO_GetBool(with_image);
    MACRO_GetString(image_data_base64);
    MACRO_GetString(public_key_base64);
    MACRO_GetString(password);

    reducedsole::uuid imageId(ExtUuid::NullUuid);
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
        deviceTokenCache.add(context.userId,
                             deviceToken);
    }
    if (password.size())
    {
        databaseLogics.databaseLogicAppUser.updatePasswordLoggedIn(context.userId, password);
    }

    t0002_user user;
    if (!user.load(context, context.userId))
    {
        answerOk("could not load user", false);
        return;
    }
    user.setfstname(fstname);
    user.setsurname(surname);
    user.setvisible_name(visible_name);
    user.setcolor(color);
    user.setmessage_font_color(message_font_color);
    user.setsearching_exactly_allowed(searching_exactly_allowed);
    user.setsearching_fuzzy_allowed(searching_fuzzy_allowed);
    user.setpublic_key_base64(public_key_base64);
    if (with_image)
    {
        user.setimage_id(imageId);
    }
    user.store(context);

    std::map<std::string, std::string> data;
    data["image_id"] = imageId.str();
    answerOk("profile updated", true, data);
}
