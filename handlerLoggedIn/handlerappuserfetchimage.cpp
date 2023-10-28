#include "handlerappuserfetchimage.h"
#include "opencvwrapper/opencvwrapper.h"
#include "utils/base64.h"

HandlerAppUserFetchImage::HandlerAppUserFetchImage(PistacheServerInterface &serverInterface,
                                                   DatabaseLogics &databaseLogics,
                                                   LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "/fetchImage",
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{
}

void HandlerAppUserFetchImage::method()
{
    MACRO_GetMandatoryUuid(imageId);
    MACRO_GetMandatoryString(imageType);
    MACRO_GetInt(desired_width);
    MACRO_GetInt(desired_height);
    if (imageType == "message")
    {
        // always allowed ??? FixMe Decide :-)
        std::basic_string<std::byte> imageData;
        std::string message;
        if (databaseLogics.databaseLogicMessages.fetchImage(imageId,
                                                            message,
                                                            imageData))
        {

            if (desired_height && desired_width)
            {
                OpenCVWrapper cv;
                cv.resizeImage(imageData, desired_width, desired_height, message);
            }
            std::map<std::string, std::string> data;
            data["imageDataBase64"] = base64_encode(reinterpret_cast<unsigned char*>(imageData.data()), imageData.size());
            answerOk(message, true, data);
            return;
        }
        answerOk(message, false);
        return;
    }
    if (imageType == "profileImage")
    {
        // always allowed
        std::basic_string<std::byte> imageData;
        std::string message;
        if (databaseLogics.databaseLogicImageTable.fetchImage(imageId,
                                                              message,
                                                              imageData))
        {

            if (desired_height && desired_width)
            {
                OpenCVWrapper cv;
                cv.resizeImage(imageData, desired_width, desired_height, message);
            }
            std::map<std::string, std::string> data;
            data["imageDataBase64"] = base64_encode(reinterpret_cast<unsigned char*>(imageData.data()), imageData.size());
            answerOk(message, true, data);
            return;
        }
        answerOk(message, false);
        return;
    }
}
