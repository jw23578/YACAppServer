#include "handlerapp.h"
#include "opencvwrapper/opencvwrapper.h"
#include "utils/base64.h"

HandlerAPP::HandlerAPP(DatabaseLogicUserAndApp &databaseLogicUserAndApp,
                       ORMPersistenceInterface &opi,
                       PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             opi,
                             TypeNoLoginNeeded),
    dlua(databaseLogicUserAndApp)
{
    addMethod(serverInterface,
              methodNames.getAPP,
              TypeGet);
    addMethod(serverInterface,
              methodNames.getAllAPPs,
              TypeGet);
    addMethod(serverInterface,
              methodNames.getAPPImage,
              TypeGet);}

void HandlerAPP::method(CurrentContext &context)
{
    if (isMethod(methodNames.getAPPImage))
    {
        MACRO_GetMandatoryUuid(imageId);
        MACRO_GetInt(desired_width);
        MACRO_GetInt(desired_height);
        std::basic_string<std::byte> imageData;
        std::string errorMessage;
        if (dlua.getAppImage(imageId,
                             imageData,
                             errorMessage))
        {
            if (desired_height && desired_width)
            {
                OpenCVWrapper cv;
                if (!cv.resizeImage(imageData, desired_width, desired_height, errorMessage))
                {
                    answerOk(errorMessage, false);
                    return;
                }
            }
            std::map<std::string, std::string> data;
            data["imageDataBase64"] = base64_encode(reinterpret_cast<unsigned char*>(imageData.data()), imageData.size());
            answerOk("here is the image", true, data);
            return;
        }
        answerOk(errorMessage, false);
        return;
    }

    if (isMethod(methodNames.getAllAPPs))
    {
        rapidjson::Document allAPPsObject;
        dlua.getAllAPPs(allAPPsObject);
        answerOk(true, allAPPsObject);
        return;
    }

    MACRO_GetMandatoryString(app_id);
    MACRO_GetMandatoryInt(current_installed_version, true);
    MACRO_GetString(installation_code);
    rapidjson::Document target;
    answerOk(dlua.fetchOneApp(app_id, current_installed_version, installation_code, target), target);
}
