#include "handlerappuserupdateprofile.h"
#include "base64.h"
#include "definitions.h"

HandlerAppUserUpdateProfile::HandlerAppUserUpdateProfile(PistacheServerInterface &serverInterface,
                                                         DatabaseLogics &databaseLogics,
                                                         LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/updateAppUserProfile",
                             TypePost,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{

}

void HandlerAppUserUpdateProfile::method()
{
    MACRO_GetMandatoryString(fstname);
    MACRO_GetMandatoryString(surname);
    MACRO_GetMandatoryString(visible_name);
    MACRO_GetMandatoryBool(searching_exactly_allowed);
    MACRO_GetMandatoryBool(searching_fuzzy_allowed);
    MACRO_GetBool(with_image);
    MACRO_GetString(image_data_base64);

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


    std::string message;
    if (!databaseLogics.databaseLogicAppUser.updateAppUser(appId,
                                                           userId,
                                                           fstname,
                                                           surname,
                                                           visible_name,
                                                           searching_exactly_allowed,
                                                           searching_fuzzy_allowed,
                                                           with_image,
                                                           imageId,
                                                           message))
    {
        answerOk(message, false);
        return;
    }
    answerOk(message, true);
}
