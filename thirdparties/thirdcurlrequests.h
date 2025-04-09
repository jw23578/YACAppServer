#ifndef THIRDCURLREQUESTS_H
#define THIRDCURLREQUESTS_H

#include <string>
#include <map>
#include <optional>
#include <chrono>
#include "logstatcontroller.h"
#include "JWUtils/reducedsole.h"

class ThirdCurlRequests
{
    struct ThirdData
    {
        std::string url;
        std::string param;
        ThirdData(const std::string &url,
                  const std::string &param):
            url(url),
            param(param)
        {
        }
    };
    typedef std::tuple<std::string, std::string> ThirdMandantType;
    const std::map<ThirdMandantType, ThirdData> thirdmandant2data{{{"aidoo", "test"}, {"https://h2877991.stratoserver.net:12345/v0001", "token=DasAidooToken&allowed_stud_nr=-1"}}};

    std::optional<ThirdData> lookupThirdData(const std::string &third, const std::string &mandant) const;

public:
    ThirdCurlRequests();
    bool login(LogStatController &logStat,
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
               std::string &loginToken);

    bool isLoggedIn(LogStatController &logStat,
                    const std::string &third,
                    const std::string &mandant,
                    const std::string &loginEMail,
                    const std::string &loginToken,
                    std::string &message,
                    std::chrono::system_clock::time_point &loginTokenValidUntil);

    void refreshLoginToken(LogStatController &logStat,
                           const std::string &third,
                           const std::string &mandant,
                           const std::string &loginEMail,
                           const std::string &loginToken,
                           std::chrono::_V2::system_clock::time_point &loginTokenValidUntil);
};

#endif // THIRDCURLREQUESTS_H
