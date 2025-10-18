#include "testviacurl.h"

#include "extstring.h"
#include "extuuid.h"
#include "orm_implementions/t0002_user.h"
#include "logstatcontroller.h"

TestViaCurl::TestViaCurl(int port, const std::string &baseUrl, ORMPersistenceInterface &opi):
    baseUrl(baseUrl + ":" + ExtString::toString(port) + "/"),
    opi(opi)
{}

bool TestViaCurl::post(jw78::CurlWrapper &cw,
                       const std::string &method,
                       const std::string &jsonPayload,
                       rapidjson::Document &resultJSON,
                       std::string &message,
                       bool expected)
{
    LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, "");
    LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, "");
    LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("TEST: -------") + method + ("-------"));
    std::string result;
    if (cw.post(baseUrl + method, jsonPayload, result, message))
    {
        LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("Post-Result: ") + result);
        resultJSON.Parse(result);
        ExtRapidJSON json(resultJSON);
        LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("<<<<<<<") + method + (">>>>>>"));
        return json.getBool("success") == expected;
    }
    LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("Post-Error: ") + message);
    LogStatController::slog(__FILE__, __LINE__, LogStatController::verbose, std::string("<<<<<<<") + method + (">>>>>>"));
    exit(1);
    return false;
}

bool TestViaCurl::testRegisterUser(const std::string &loginEMail,
                                   const std::string &password)
{
    jw78::CurlWrapper cw;
    rapidjson::Document resultJSON;
    std::string message;
    cw.addHeader("Content-Type", "application/json");
    cw.addHeader("YACAPP-AppId", appId.str());
    std::string jsonString;
    {
        rapidjson::Document data;
        data.SetObject();
        ExtRapidJSONWriter w(data, data.GetAllocator());
        w.addMember("loginEMail", loginEMail);
        w.addMember("password", password);
        jsonString = w.toString();
    }
    return post(cw, methodNames.registerUser, jsonString, resultJSON, message, true);
}

bool TestViaCurl::run(std::string &resultMessage)
{
    Log::Scope scope("TestViaCurl");
    CurrentContext context(opi, appId, NullUuid);
    rapidjson::Document resultJSON;
    std::string message;
    cw.addHeader("Content-Type", "application/json");
    cw.addHeader("YACAPP-AppId", appId.str());
    std::string loginEMail("jens@wienoebst.com");
    std::string password("my_password");
    std::string jsonString;

    testRegisterUser(loginEMail, password);

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
    if (!post(cw, methodNames.verifyUser, jsonString, resultJSON, message, true))
    {
        resultMessage = methodNames.verifyUser + " did not work";
        return false;
    }
    ExtRapidJSON json(resultJSON);
    std::string loginToken(json.getString("loginToken"));

    cw.addHeader("YACAPP-LoginEMail", loginEMail);
    cw.addHeader("YACAPP-LoginToken", loginToken);
    if (!post(cw, methodNames.logoutUser, jsonString, resultJSON, message, true))
    {
        resultMessage = methodNames.logoutUser + " did not work";
        return false;
    }

    {
        rapidjson::Document data;
        data.SetObject();
        ExtRapidJSONWriter w(data, data.GetAllocator());
        w.addMember("loginEMail", loginEMail);
        w.addMember("password", password);
        jsonString = w.toString();
    }
    if (!post(cw, methodNames.updatePasswordUser, jsonString, resultJSON, message, false))
    {
        resultMessage = methodNames.updatePasswordUser + " did not work";
        return false;
    }


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
    if (!post(cw, methodNames.updateUserProfile, jsonString, resultJSON, message, false))
    {
        resultMessage = methodNames.updateUserProfile + " did not work";
        return false;
    }

    {
        rapidjson::Document data;
        data.SetObject();
        ExtRapidJSONWriter w(data, data.GetAllocator());
        w.addMember("loginEMail", loginEMail);
        w.addMember("password", password);
        jsonString = w.toString();
    }
    if (!post(cw, methodNames.loginUser, jsonString, resultJSON, message, true))
    {
        resultMessage = methodNames.loginUser + " did not work";
        return false;
    }
    return true;
}
