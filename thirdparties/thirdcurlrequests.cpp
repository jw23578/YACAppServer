#include "thirdcurlrequests.h"
#include "curlWrapper/jw78curlwrapper.h"
#include "extrapidjson.h"
#include "utils/extstring.h"

std::optional<ThirdCurlRequests::ThirdData> ThirdCurlRequests::lookupThirdData(const std::string &third, const std::string &mandant) const
{
    const auto it(thirdmandant2data.find({third, mandant}));
    if (it == thirdmandant2data.end())
    {
        return {};
    }
    return it->second;
}

ThirdCurlRequests::ThirdCurlRequests()
{

}

bool ThirdCurlRequests::login(LogStatController &logStat,
                              const std::string &third,
                              const std::string &mandant,
                              const std::string &loginEMail,
                              const std::string &password,
                              std::string &message,
                              std::string &fstname,
                              std::string &surname,
                              std::string &visible_name,
                              std::string &user_id_string,
                              std::string &image_id_string,
                              std::string &loginToken)
{
    std::optional<ThirdCurlRequests::ThirdData> thirdData(lookupThirdData(third, mandant));
    if (!thirdData.has_value())
    {
        message = "third/mandant combination are unknown";
        return false;
    }
    if (third == "aidoo")
    {
        jw78::CurlWrapper cw;
        std::string url(thirdData.value().url + "/login?memberData=1&");
        url += thirdData.value().param;
        url += std::string("&pwd=") + password;
        url += std::string("&login=") + loginEMail;
        std::string result;
        logStat.log(__FILE__, __LINE__, LogStatController::verbose, std::string("url: ") + url);
        if (!cw.get(url, result, message))
        {
            message = std::string("could not connect to aidoo mandant: ") + mandant;
            return false;
        }
        rapidjson::Document document;
        document.Parse(result);
        ExtRapidJSON ext(document);
        ExtRapidJSON memberData(document["memberData"]);
        logStat.log(__FILE__, __LINE__, LogStatController::verbose, result);
        std::string message(ext.getString("message"));
        if (message.size())
        {
            return false;
        }
        fstname = memberData.getString("name");
        surname = memberData.getString("surname");
        visible_name = fstname + " " + surname;
        logStat.log(__FILE__, __LINE__, LogStatController::verbose, fstname);
        logStat.log(__FILE__, __LINE__, LogStatController::verbose, surname);

        user_id_string = ExtString::toString(ext.getInt("mitg_nr"));
        image_id_string = ext.getString("imageid");
        loginToken = ext.getString("loginToken");
        return true;
    }
    message = std::string("unknown third party: ") + third;
    return false;
}

bool ThirdCurlRequests::isLoggedIn(LogStatController &logStat,
                                   const std::string &third,
                                   const std::string &mandant,
                                   const std::string &loginEMail,
                                   const std::string &loginToken,
                                   std::string &message,
                                   std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::optional<ThirdCurlRequests::ThirdData> thirdData(lookupThirdData(third, mandant));
    if (!thirdData.has_value())
    {
        message = "third/mandant combination are unknown";
        return false;
    }
    if (third == "aidoo")
    {
        // FIXME, offensichtlich muss das noch implementiert werden
        loginTokenValidUntil = std::chrono::system_clock::now() + std::chrono::hours(24 * 7 * 6);
        return true;
    }
    return false;
}

void ThirdCurlRequests::refreshLoginToken(LogStatController &logStat,
                                          const std::string &third,
                                          const std::string &mandant,
                                          const std::string &loginEMail,
                                          const std::string &loginToken,
                                          std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::optional<ThirdCurlRequests::ThirdData> thirdData(lookupThirdData(third, mandant));
    if (!thirdData.has_value())
    {
        return;
    }
    if (third == "aidoo")
    {
        // FIXME, offensichtlich muss das noch implementiert werden
        int validHours(24 * 7);
        loginTokenValidUntil = std::chrono::system_clock::now() + std::chrono::hours(1) * validHours;
        return;
    }
}
