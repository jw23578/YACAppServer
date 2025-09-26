#include "handleruser.h"
#include "extrapidjson.h"
#include "thirdparties/thirdcurlrequests.h"
#include "serverHeader/thirdheader.h"
#include "serverHeader/mandantheader.h"
#include "extmap.h"

bool HandlerUser::thirdLogin(const reducedsole::uuid &appId,
                             const std::string &third,
                             const std::string &mandant,
                             const std::string &loginEMail,
                             const std::string &password,
                             std::string &message,
                             ExtRapidJSONWriter &w,
                             reducedsole::uuid &appUserId)
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
        t0002_user user;
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

HandlerUser::HandlerUser(LoggedInAppUsersContainer &loggedInUsersContainer,
                         EMailLogic &emailLogic,
                         DatabaseLogics &databaseLogics,
                         DeviceTokenCache &deviceTokenCache,
                         PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             TypeNoLoginNeeded),
    loggedInUsersContainer(loggedInUsersContainer),
    emailLogic(emailLogic),
    databaseLogics(databaseLogics),
    deviceTokenCache(deviceTokenCache)
{
    addMethod(serverInterface, methodNames.loginUser, TypePost);
    addMethod(serverInterface, methodNames.userLoggedIn, TypeGet);
    addMethod(serverInterface, methodNames.registerUser, TypePost);
    addMethod(serverInterface, methodNames.requestVerifyToken, TypePost);
    addMethod(serverInterface, methodNames.verifyUser, TypePost);
    addMethod(serverInterface, methodNames.updatePasswordUser, TypePost);
    addMethod(serverInterface, methodNames.requestUpdatePasswordUser, TypePost);
}

void HandlerUser::method()
{
    if (isMethod(methodNames.requestUpdatePasswordUser))
    {
        MACRO_GetMandatoryEMail(loginEMail);

        std::string message;
        std::string updatePasswordToken;
        if (!databaseLogics.databaseLogicAppUser.requestUpdatePassword(getAppId(),
                                                                       loginEMail,
                                                                       updatePasswordToken,
                                                                       message))
        {
            answerBad(message);
            return;
        }
        emailLogic.sendPleaseUpdatePasswordMail(loginEMail, updatePasswordToken);
        message = "e-mail with updatePasswordToken sended";
        answerOk(message, true);
        return;
    }
    if (isMethod(methodNames.updatePasswordUser))
    {
        MACRO_GetMandatoryEMail(loginEMail);
        MACRO_GetMandatoryString(password);
        MACRO_GetMandatoryString(updatePasswordToken);

        std::string message;
        std::string loginToken;
        reducedsole::uuid userId;
        if (!databaseLogics.databaseLogicAppUser.updatePassword(getAppId(),
                                                                loginEMail,
                                                                updatePasswordToken,
                                                                password,
                                                                message,
                                                                loginToken,
                                                                userId))
        {
            answerBad(message);
            return;
        }
        loggedInUsersContainer.clear(userId);
        MACRO_CreateDataMAP(loginToken);
        answerOk(message, true, data);
        return;
    }
    if (isMethod(methodNames.verifyUser))
    {
        MACRO_GetMandatoryEMail(loginEMail);
        MACRO_GetMandatoryString(verifyToken);

        std::string message;
        rapidjson::Document data;
        data.SetObject();
        ExtRapidJSONWriter w(data, data.GetAllocator());
        reducedsole::uuid appUserId;
        if (!databaseLogics.databaseLogicAppUser.verifyUser(getAppId(),
                                                               loginEMail,
                                                               verifyToken,
                                                               message,
                                                               w,
                                                               appUserId))
        {
            answerOk(message, false);
            return;
        }
        w.addMember("message", message);
        databaseLogics.rightsLogic.addUserRights(getAppId(), appUserId, data, data.GetAllocator());
        answerOk(true, data);
        return;
    }
    if (isMethod(methodNames.requestVerifyToken))
    {
        MACRO_GetMandatoryEMail(loginEMail);
        std::string verifyToken;
        std::string message;
        if (!databaseLogics.databaseLogicAppUser.createVerifyToken(getAppId(),
                                                                   loginEMail,
                                                                   message,
                                                                   verifyToken))
        {
            answerOk(message, false);
            return;
        }
        emailLogic.sendVerifyTokenMail(loginEMail, verifyToken);
        answerOk("verifyToken created and sended",
                 true);
        return;
    }
    if (isMethod(methodNames.registerUser))
    {
        MACRO_GetMandatoryEMail(loginEMail);
        MACRO_GetString(password);


        std::string verifyToken;
        std::string message;
        if (!databaseLogics.databaseLogicAppUser.createAppUser(getAppId(),
                                                               loginEMail,
                                                               password,
                                                               message,
                                                               verifyToken))
        {
            answerOk(message, false);
            return;
        }
        emailLogic.sendPleaseVerifyMail(loginEMail, verifyToken);
        answerOk("appuser registered, please verify", true);
        return;
    }
    if (isMethod(methodNames.userLoggedIn))
    {
        MACRO_GetMandatoryEMail(loginEMail);
        MACRO_GetMandatoryString(loginToken);

        std::string third;
        std::string mandant;
        getHeaderString<ThirdHeader>(third, false);
        getHeaderString<MandantHeader>(mandant, false);


        if (loggedInUsersContainer.isLoggedInWithOutUserId(getAppId(),
                                                           loginEMail,
                                                           loginToken,
                                                           third,
                                                           mandant))
        {
            answerOk("user logged in", true);
            return;
        }
        answerOk("user not logged in", false);
        return;
    }

    MACRO_GetMandatoryString(password);

    std::string third;
    std::string mandant;
    getHeaderString<ThirdHeader>(third, false);
    getHeaderString<MandantHeader>(mandant, false);
    MACRO_GetMandatoryLogin(loginEMail, third);

    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, std::string("third: ") + third);
    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, std::string("mandant: ") + mandant);

    std::string message;
    reducedsole::uuid userId;
    rapidjson::Document data;
    data.SetObject();
    ExtRapidJSONWriter w(data, data.GetAllocator());

    bool loggedIn(false);
    if (third.size())
    {
        loggedIn = thirdLogin(getAppId(),
                              third,
                              mandant,
                              loginEMail,
                              password,
                              message,
                              w,
                              userId);
    }
    else
    {
        loggedIn = databaseLogics.databaseLogicAppUser.loginUser(getAppId(),
                                                                    loginEMail,
                                                                    password,
                                                                    message,
                                                                    w,
                                                                    userId);
    }
    if (!loggedIn)
    {
        answerOk(message, false);
        return;
    }
    MACRO_GetString(deviceToken);
    if (deviceToken.size())
    {
        deviceTokenCache.add(userId,
                             deviceToken);
    }
    w.addMember("message", "Login successful");
    RightsLogic &rl(databaseLogics.rightsLogic);
    rl.addUserRights(getAppId(), userId, data, data.GetAllocator());
    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, data);
    answerOk(true, data);
}
