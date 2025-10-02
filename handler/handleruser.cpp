#include "handleruser.h"
#include "extrapidjson.h"
#include "thirdparties/thirdcurlrequests.h"
#include "serverHeader/thirdheader.h"
#include "serverHeader/mandantheader.h"
#include "extmap.h"
#include "orm_implementions/t0003_user_passwordhashes.h"
#include "orm_implementions/t0004_user_logintoken.h"

bool HandlerUser::thirdLogin(CurrentContext &context,
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
    if (!databaseLogics.databaseLogicThirdParty.lookup(context.appId,
                                                       third,
                                                       mandant,
                                                       user_id_string,
                                                       thirdPartyUser))
    {
        const bool searching_exactly_allowed(true);
        const bool searching_fuzzy_allowed(true);
        const std::string public_key_base64; // FIXME
        t0002_user user;
        if (!user.createVerifiedAppUser(context,
                                        loginEMail,
                                        fstname,
                                        surname,
                                        visible_name,
                                        searching_exactly_allowed,
                                        searching_fuzzy_allowed,
                                        public_key_base64,
                                        message))
        {
            return false;
        }
        databaseLogics.databaseLogicThirdParty.create(context.appId,
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
                             databaseLogics.getOpi(),
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

void HandlerUser::method(CurrentContext &context)
{
    if (isMethod(methodNames.requestUpdatePasswordUser))
    {
        MACRO_GetMandatoryEMail(loginEMail);

        std::string message;
        t0002_user user;
        if (!user.requestUpdatePassword(context,
                                        loginEMail,
                                        message))
        {
            answerBad(message);
            return;
        }
        emailLogic.sendPleaseUpdatePasswordMail(loginEMail, user.update_password_token);
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
        t0002_user user;
        if (!user.updatePassword(context,
                                 loginEMail,
                                 updatePasswordToken,
                                 password,
                                 message,
                                 loginToken))
        {
            answerBad(message);
            return;
        }
        loggedInUsersContainer.clear(user.user_id);
        MACRO_CreateDataMAP(loginToken);
        answerOk(message, true, data);
        return;
    }
    if (isMethod(methodNames.verifyUser))
    {
        MACRO_GetMandatoryEMail(loginEMail);
        MACRO_GetMandatoryString(verifyToken);

        std::string message;
        std::string loginToken;
        t0002_user userToVerify;
        if (!userToVerify.verifyUser(context,
                                     loginEMail,
                                     verifyToken,
                                     loginToken,
                                     message))
        {
            answerOk(message, false);
            return;
        }

        rapidjson::Document data;
        data.SetObject();
        ExtRapidJSONWriter w(data, data.GetAllocator());
        w.addMember("loginToken", loginToken);
        w.addMember(userToVerify.user_id.name(), userToVerify.user_id.asString());
        w.addMember(userToVerify.fstname.name(), userToVerify.fstname);
        w.addMember(userToVerify.surname.name(), userToVerify.surname);
        w.addMember(userToVerify.visible_name.name(), userToVerify.visible_name);
        w.addMember("message", message);
        databaseLogics.rightsLogic.addUserRights(context, data, data.GetAllocator());
        answerOk(true, data);
        return;
    }
    if (isMethod(methodNames.requestVerifyToken))
    {
        MACRO_GetMandatoryEMail(loginEMail);
        std::string message;
        t0002_user user;
        if (!user.createVerifyToken(context,
                                    loginEMail,
                                    message))
        {
            answerOk(message, false);
            return;
        }
        emailLogic.sendVerifyTokenMail(loginEMail, user.verify_token);
        answerOk("verifyToken created and sended",
                 true);
        return;
    }
    if (isMethod(methodNames.registerUser))
    {
        MACRO_GetMandatoryEMail(loginEMail);
        MACRO_GetString(password);

        t0002_user newUser;
        std::string message;
        if (!newUser.registerUser(context,
                                  loginEMail,
                                  password,
                                  message))
        {
            answerOk(message, false);
            return;
        }
        emailLogic.sendPleaseVerifyMail(loginEMail, newUser.verify_token);
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


        if (loggedInUsersContainer.isLoggedInWithOutUserId(context,
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
    rapidjson::Document data;
    data.SetObject();
    ExtRapidJSONWriter w(data, data.GetAllocator());

    bool loggedIn(false);
    if (third.size())
    {
        loggedIn = thirdLogin(context,
                              third,
                              mandant,
                              loginEMail,
                              password,
                              message,
                              w,
                              context.userId);
    }
    else
    {
        t0002_user user;
        if (!user.lookupUser(context, loginEMail, message))
        {
            answerOk("LoginEMail/User not found. Please check your LoginEMail or register first.", false);
            return;
        }
        context.userId = user.user_id;
        t0003_user_passwordhashes userPasswordhash;
        if (!userPasswordhash.login(context,
                                    context.userId,
                                    password,
                                    message))
        {
            answerOk(message, false);
            return;
        }
        t0004_user_logintoken userLogintoken;
        userLogintoken.loginSuccessful(context, context.userId);
        w.addMember("loginToken", userLogintoken.login_token);
        w.addMember(user.user_id.name(), user.user_id.asString());
        w.addMember(user.fstname.name(), user.fstname);
        w.addMember(user.surname.name(), user.surname);
        w.addMember(user.visible_name.name(), user.visible_name);
        w.addMember(user.image_id.name(), user.image_id.asString());
        loggedIn = true;
    }
    if (!loggedIn)
    {
        answerOk(message, false);
        return;
    }
    MACRO_GetString(deviceToken);
    if (deviceToken.size())
    {
        deviceTokenCache.add(context,
                             deviceToken);
    }
    w.addMember("message", "Login successful");
    RightsLogic &rl(databaseLogics.rightsLogic);
    rl.addUserRights(context, data, data.GetAllocator());
    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, data);
    answerOk(true, data);
}
