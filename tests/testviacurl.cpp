#include "testviacurl.h"

#include "extstring.h"
#include "extuuid.h"
#include "extrapidjson.h"
#include "yacappservermethodnames.h"
#include "orm_implementions/t0002_user.h"
#include "logstatcontroller.h"

TestViaCurl::TestViaCurl(int port, const std::string &baseUrl, ORMPersistenceInterface &opi):
    baseUrl(baseUrl + ":" + ExtString::toString(port) + "/"),
    opi(opi)
{}

bool TestViaCurl::post(const std::string &url,
                       const std::string &jsonPayload,
                       std::string &result,
                       std::string &message)
{
    if (cw.post(url, jsonPayload, result, message))
    {
        LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("Post-Result: ") + result);
        return true;
    }
    LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("Post-Error: ") + message);
    return false;
}

bool TestViaCurl::run(std::string &resultMessage)
{
    Log::Scope scope("TestViaCurl");
    YACAPPServerMethodNames methodNames;
    reducedsole::uuid appId = ExtUuid::stringToUuid("00000000-0000-0000-0000-000000000001");
    CurrentContext context(opi, appId, NullUuid);
    std::string result;
    std::string message;
    cw.addHeader("Content-Type", "application/json");
    cw.addHeader("YACAPP-AppId", appId.str());
    std::string loginEMail("jens@wienoebst.com");
    std::string password("my_password");
    std::string jsonString;
    {
        rapidjson::Document data;
        data.SetObject();
        ExtRapidJSONWriter w(data, data.GetAllocator());
        w.addMember("loginEMail", loginEMail);
        w.addMember("password", password);
        jsonString = w.toString();
    }
    if (!post(baseUrl + methodNames.registerUser, jsonString, result, message))
    {
        return false;
    }
    t0002_user userProfile;
    userProfile.loadByLoginEMail(context, loginEMail);
    {
        rapidjson::Document data;
        data.SetObject();
        ExtRapidJSONWriter w(data, data.GetAllocator());
        w.addMember("loginEMail", loginEMail);
        w.addMember("verifyToken", userProfile.verify_token);
        jsonString = w.toString();
    }
    if (!post(baseUrl + methodNames.verifyUser, jsonString, result, message))
    {
        resultMessage = methodNames.verifyUser + " did not work";
        return false;
    }
    rapidjson::Document resultJSON;
    resultJSON.Parse(result);
    ExtRapidJSON json(resultJSON);
    std::string loginToken(json.getString("loginToken"));

    cw.addHeader("YACAPP-LoginEMail", loginEMail);
    cw.addHeader("YACAPP-LoginToken", loginToken);
    {
        rapidjson::Document data;
        data.SetObject();
        ExtRapidJSONWriter w(data, data.GetAllocator());
        w.addMember("fstname", "Jens");
        w.addMember("surname", "Jens");
        w.addMember("visible_name", "Jens W.");
        w.addMember("color", "#fff");
        w.addMember("message_font_color", "#fff");
        w.addMember("searching_exactly_allowed", true);
        w.addMember("searching_fuzzy_allowed", true);
        jsonString = w.toString();
    }
    if (!post(baseUrl + methodNames.updateAppUserProfile, jsonString, result, message))
    {
        resultMessage = methodNames.updateAppUserProfile + " did not work";
        return false;
    }
    return true;
}
