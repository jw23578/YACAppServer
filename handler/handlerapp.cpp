#include "handlerapp.h"
#include "opencvwrapper/opencvwrapper.h"
#include "utils/base64.h"
#include "orm_implementions/t0001_apps.h"
#include "orm_implementions/t0009_largeobject.h"
#include "orm_implementions/t0010_largeobject2object.h"

HandlerAPP::HandlerAPP(ORMPersistenceInterface &opi,
                       PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             opi,
                             TypeNoLoginNeeded)
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
        t0009_largeobject appImage;
        if (!appImage.load(context, imageId))
        {
            answerOk("could not find appImage", false);
            return;
        }
        if (!context.opi.fetchBlob(appImage.database_blob_id, imageData))
        {
            answerOk("could not load imageData", false);
            return;
        }

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
        data["imageDataBase64"] = base64_encode(imageData);
        answerOk("here is the image", true, data);
        return;
    }

    if (isMethod(methodNames.getAllAPPs))
    {
        ORMVector<t0001_apps> allAppsVector;
        t0001_apps::getAllAPPs(context, allAppsVector);

        rapidjson::Document allAPPsObject;
        auto &alloc(allAPPsObject.GetAllocator());
        rapidjson::Value allAPPs(rapidjson::kArrayType);
        for (size_t r(0); r < allAppsVector.size(); ++r)
        {
            t0001_apps &app(allAppsVector[r]);
            rapidjson::Value appObject;
            appObject.SetObject();
            ExtRapidJSONWriter ao(appObject, alloc);
            ao.addMember(app.search_codeORM().name(), app.search_code.asString());
            ao.addMember("installation_code_needed", app.installation_code_hash.asString().size() > 0);
            ao.addMember(app.app_info_urlORM().name(), app.app_info_url.asString());
            ao.addMember(app.app_idORM().name(), app.app_id.asString());
            ao.addMember(app.app_nameORM().name(), app.app_name.asString());
            ao.addMember(app.app_versionORM().name(), app.app_version.asString());
            ao.addMember(app.app_logo_urlORM().name(), app.app_logo_url.asString());
            ao.addMember(app.app_color_nameORM().name(), app.app_color_name.asString());

            rapidjson::Value json_app_images;
            json_app_images.SetArray();
            for (size_t i(0); i < app.app_image_ids.valueSize(); ++i)
            {
                rapidjson::Value v(app.app_image_ids.getValue(i), alloc);
                json_app_images.PushBack(v, alloc);
            }
            appObject.AddMember("app_images", json_app_images, alloc);
            allAPPs.PushBack(appObject, alloc);
        }
        allAPPsObject.SetObject();
        allAPPsObject.AddMember("allApps", allAPPs, alloc);
        answerOk(true, allAPPsObject);
        return;
    }

    MACRO_GetMandatoryInt(current_installed_version, true);
    MACRO_GetString(installation_code);

    t0001_apps app;
    std::string message;
    if (!app.fetchOneApp(context, current_installed_version, installation_code, message))
    {
        answerOk("app not found", false);
        return;
    }
    if (!app.installation_code_ok)
    {
        answerOk("wrong installation code", false);
        return;
    }
    if (app.app_version <= current_installed_version)
    {
        answerOk("pp version is up to date", false);
        return;
    }
    rapidjson::Document target;
    ExtRapidJSONWriter json(target, target.GetAllocator());
    json.addMember(app.app_idORM().name(), app.app_id.asString());
    json.addMember(app.app_nameORM().name(), app.app_name.asString());
    json.addMember("message", "app found");
    json.addMember(app.json_yacappORM().name(), app.json_yacapp.asString());

    std::string yacpck_base64;
    context.opi.fetchBlob(app.yacpck_base64.get(), yacpck_base64);
    json.addMember(app.yacpck_base64ORM().name(), yacpck_base64);

    answerOk(true, target);

    //answerOk(dlua.fetchOneApp(context.appId.str(), current_installed_version, installation_code, target), target);
}
