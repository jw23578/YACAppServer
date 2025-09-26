#include "testviacurl.h"

#include "curlWrapper/jw78curlwrapper.h"
#include "extstring.h"
#include "extuuid.h"
#include "extrapidjson.h"
#include "yacappservermethodnames.h"
#include "orm_implementions/t0002_user.h"

TestViaCurl::TestViaCurl(int port, const std::string &baseUrl, ORMPersistenceInterface &opi):
    baseUrl(baseUrl + ":" + ExtString::toString(port) + "/"),
    opi(opi)
{}

bool TestViaCurl::run(std::string &resultMessage)
{
    YACAPPServerMethodNames methodNames;
    reducedsole::uuid appId = ExtUuid::stringToUuid("00000000-0000-0000-0000-000000000001");
    std::string result;
    std::string message;
    jw78::CurlWrapper cw;
    cw.addHeader("Content-Type", "application/json");
    cw.addHeader("YACAPP-AppId", appId.str());
    if (!cw.post(baseUrl + methodNames.registerUser,
            "{\"loginEMail\":\"jens@wienoebst.com\",\"password\":\"my_password\"}",
            result,
                 message))
    {
        resultMessage = methodNames.registerUser + " did not work";
        return false;
    }
    t0002_user userProfile;
    userProfile.loadByLoginEMail(opi, appId, "jens@wienoebst.com");
    if (!cw.post(baseUrl + methodNames.verifyUser,
                 std::string("{\"loginEMail\":\"jens@wienoebst.com\",\"verifyToken\":") + ExtString::quote(userProfile.verify_token) + "}",
                 result,
                 message))
    {
        resultMessage = methodNames.verifyUser + " did not work";
        return false;
    }
    rapidjson::Document resultJSON;
    resultJSON.Parse(result);
    ExtRapidJSON json(resultJSON);
    return true;
}
