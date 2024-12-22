#include "handlerappuserlogin.h"
#include "extrapidjson.h"
#include "thirdparties/thirdcurlrequests.h"
#include "serverHeader/appidheader.h"
#include "serverHeader/thirdheader.h"
#include "serverHeader/mandantheader.h"

bool HandlerAppUserLogin::thirdLogin(const sole::uuid &appId,
                                     const std::string &third,
                                     const std::string &mandant,
                                     const std::string &loginEMail,
                                     const std::string &password,
                                     std::string &message,
                                     ExtRapidJSONWriter &w,
                                     sole::uuid &appUserId)
{

    std::string fstname;
    std::string surname;
    std::string visible_name;
    std::string user_id_string;
    std::string image_id_string;
    std::string loginToken;
    ThirdCurlRequests tcr;
    if (!tcr.login(databaseLogics.getLogStat(),
                   third,
                   mandant,
                   loginEMail,
                   password,
                   message,
                   fstname,
                   surname,
                   visible_name,
                   user_id_string,
                   image_id_string,
                   loginToken))
    {
        return false;
    }

    t0029_third_party_user_data thirdPartyUser;
    if (!databaseLogics.databaseLogicThirdParty.lookup(appId,
                                                       third,
                                                       mandant,
                                                       user_id_string,
                                                       thirdPartyUser))
    {
        const bool searching_exactly_allowed(true);
        const bool searching_fuzzy_allowed(true);
        const std::string public_key_base64; // FIXME
        t0003_appuser_profiles user;
        if (!databaseLogics.databaseLogicAppUser.createVerifiedAppUser(appId,
                                                                       loginEMail,
                                                                       fstname,
                                                                       surname,
                                                                       visible_name,
                                                                       searching_exactly_allowed,
                                                                       searching_fuzzy_allowed,
                                                                       public_key_base64,
                                                                       message,
                                                                       user))
        {
            return false;
        }
        databaseLogics.databaseLogicThirdParty.create(appId,
                                                      third,
                                                      mandant,
                                                      user_id_string,
                                                      image_id_string,
                                                      user.id,
                                                      thirdPartyUser);
    }
    if (thirdPartyUser.image_id_string.get() != image_id_string)
    {
        thirdPartyUser.image_id_string = image_id_string;
        databaseLogics.databaseLogicThirdParty.update(thirdPartyUser);
    }

    appUserId = thirdPartyUser.appuser_id;

    TableFields tableFields;
    w.addMember("loginToken", loginToken);
    w.addMember(tableFields.id, thirdPartyUser.appuser_id.asString());
    w.addMember(tableFields.fstname, fstname);
    w.addMember(tableFields.surname, surname);
    w.addMember(tableFields.visible_name, visible_name);
    w.addMember(tableFields.image_id, "");
    return true;
}

HandlerAppUserLogin::HandlerAppUserLogin(DatabaseLogics &databaseLogics,
                                         DeviceTokenCache &deviceTokenCache,
                                         PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/loginAppUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogics(databaseLogics),
    deviceTokenCache(deviceTokenCache)
{

}

void HandlerAppUserLogin::method()
{
    MACRO_GetMandatoryString(password);
    sole::uuid appId;
    if (!getHeaderUuid<AppIdHeader>(appId, true))
    {
        return;
    }

    std::string third;
    std::string mandant;
    getHeaderString<ThirdHeader>(third, false);
    getHeaderString<MandantHeader>(mandant, false);
    MACRO_GetMandatoryLogin(loginEMail, third);

    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, std::string("third: ") + third);
    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, std::string("mandant: ") + mandant);

    std::string message;
    sole::uuid appUserId;
    rapidjson::Document data;
    data.SetObject();
    ExtRapidJSONWriter w(data, data.GetAllocator());

    bool loggedIn(false);
    if (third.size())
    {
        loggedIn = thirdLogin(appId,
                              third,
                              mandant,
                              loginEMail,
                              password,
                              message,
                              w,
                              appUserId);
    }
    else
    {
        loggedIn = databaseLogics.databaseLogicAppUser.loginAppUser(appId,
                                                                    loginEMail,
                                                                    password,
                                                                    message,
                                                                    w,
                                                                    appUserId);
    }
    if (!loggedIn)
    {
        answerOk(message, false);
        return;
    }
    MACRO_GetString(deviceToken);
    if (deviceToken.size())
    {
        deviceTokenCache.add(appUserId,
                             deviceToken);
    }
    w.addMember("message", "Login successful");
    RightsLogic &rl(databaseLogics.rightsLogic);
    rl.addUserRights(appId, appUserId, data, data.GetAllocator());
    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, data);
    answerOk(true, data);
}
