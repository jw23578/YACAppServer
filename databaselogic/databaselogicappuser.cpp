#include "databaselogicappuser.h"
#include "pgexecutor.h"
#include "JWUtils/definitions.h"
#include "orm_implementions/t0009_appuser_logintoken.h"
#include "orm-mapper/orm2postgres.h"
#include "logstatcontroller.h"
#include "JWUtils/extuuid.h"
#include "orm_implementions/t0004_appuser_passwordhashes.h"

void DatabaseLogicAppUser::loginSuccessful(const reducedsole::uuid &appUserId,
                                           std::string &loginToken)
{
    if (loginToken.size() == 0)
    {
        loginToken = ExtUuid::generateUuid().str();
    }
    int validHours(24 * 7);
    t0009_appuser_logintoken t0009;
    t0009.prepareFirstInsert();
    t0009.appuser_id.set(appUserId);
    t0009.login_token.set(loginToken);
    t0009.login_token_valid_until.set(std::chrono::system_clock::now() + std::chrono::hours(1) * validHours);
    opi.insertObject(t0009, appUserId);
}

bool DatabaseLogicAppUser::lookupAppUser(const reducedsole::uuid &appId,
                                         const std::string &loginEMail,
                                         reducedsole::uuid &appUserId,
                                         std::string &message)
{
    auto it(loginEMailAndAppId2AppUserId.find(loginEMail + appId.str()));
    if (it != loginEMailAndAppId2AppUserId.end())
    {
        appUserId = it->second;
        return true;
    }
    t0003_appuser_profiles t0003;
    if (!opi.selectObject({{t0003.loginemail.name(), loginEMail},
                           {t0003.app_id.name(), appId.str()}},t0003))
    {
        message = "LoginEMail/User not found. Please check your LoginEMail or register first.";
        return false;
    }
    if (t0003.verified.isNull())
    {
        message = "LoginEMail/User not yet verified";
        return false;
    }
    return true;
}

DatabaseLogicAppUser::DatabaseLogicAppUser(LogStatController &logStatController,
                                           PGConnectionPool &pool,
                                           ORMPersistenceInterface &opi):
    logStatController(logStatController),
    pool(pool),
    opi(opi)
{

}

reducedsole::uuid DatabaseLogicAppUser::getAppUserId(const reducedsole::uuid &appId,
                                                     const std::string &loginEMail)
{
    t0003_appuser_profiles t0003;
    std::map<ORMString, ORMString> field2needle;
    field2needle[tableFields.loginemail] = loginEMail;
    field2needle[tableFields.app_id] = appId.str();
    if (!opi.selectObject(field2needle, t0003))
    {
        return ExtUuid::NullUuid;
    }
    return t0003.getappuser_profile_id();
}

bool DatabaseLogicAppUser::createAppUser(reducedsole::uuid const &appId,
                                         const std::string &loginEMail,
                                         const std::string &password,
                                         std::string &message,
                                         std::string &verifyToken)
{
    Log::Scope scope("createAppUser");
    t0002_apps t0002;
    if (!opi.existObject(appId, t0002))
    {
        message = "App with app_id: " + appId.str() + " does not exist";
        return false;
    }
    if (getAppUserId(appId,
                     loginEMail) != ExtUuid::NullUuid)
    {
        message = "LoginEMail already exists.";
        return false;
    }
    reducedsole::uuid appUserId(reducedsole::uuid4());
    verifyToken = ExtString::randomString(0, 0, 4, 0);
    {
        t0003_appuser_profiles t0003;
        t0003.id = appUserId;
        t0003.app_id = appId;
        t0003.fstname = "";
        t0003.surname = "";
        t0003.visible_name = "";
        t0003.verified.setNull(true);
        t0003.loginemail = loginEMail;
        t0003.verify_token = verifyToken;
        t0003.verify_token_valid_until = std::chrono::system_clock::now() + std::chrono::minutes(60);
        t0003.update_password_token =  "";
        t0003.update_password_token_valid_until.setNull(true);
        t0003.deleted.setNull(true);
        t0003.searching_exactly_allowed = false; // FIXME
        t0003.searching_fuzzy_allowed = true; // FIXME
        t0003.public_key_base64 = ""; // FIXME
        t0003.image_id.setNull(true); // FIXME
        opi.insertObject(t0003, appUserId);
    }

    if (password.size())
    {
        t0004_appuser_passwordhashes t0004;
        t0004.appuser_id = appUserId;
        t0004.prepareFirstInsert();
        t0004.password_hash = password;
        opi.insertObject(t0004, appUserId);
    }

    return true;
}

bool DatabaseLogicAppUser::createVerifiedAppUser(const reducedsole::uuid &appId,
                                                 const std::string &loginEMail,
                                                 const std::string &fstname,
                                                 const std::string &surname,
                                                 const std::string &visible_name,
                                                 const bool searching_exactly_allowed,
                                                 const bool searching_fuzzy_allowed,
                                                 const std::string &public_key_base64,
                                                 std::string &message,
                                                 t0003_appuser_profiles &target)
{
    if (getAppUserId(appId,
                     loginEMail) != ExtUuid::NullUuid)
    {
        message = "LoginEMail already exists.";
        return false;
    }
    target.app_id = appId;
    target.fstname = fstname;
    target.surname = surname;
    target.visible_name = visible_name;
    target.verified = std::chrono::system_clock::now();
    target.loginemail = loginEMail;
    target.verify_token = "";
    target.verify_token_valid_until.setNull(true);
    target.update_password_token = "";
    target.update_password_token_valid_until.setNull(true);
    target.deleted.setNull(true);
    target.searching_exactly_allowed = searching_exactly_allowed;
    target.searching_fuzzy_allowed = searching_fuzzy_allowed;
    target.public_key_base64 = public_key_base64;
    target.image_id.setNull(true);
    target.prepareFirstInsert();
    opi.insertObject(target, target.getappuser_profile_id());
    return true;
}

bool DatabaseLogicAppUser::createVerifyToken(const reducedsole::uuid &appId,
                                             const std::string &loginEMail,
                                             std::string &message,
                                             std::string &verifyToken)
{
    t0003_appuser_profiles t0003;
    if (!opi.selectObject({{t0003.loginemail.name(), loginEMail},
                           {t0003.app_id.name(), appId.str()}},
                          t0003))
    {
        message = "LoginEMail does not exist.";
        return false;
    }
    verifyToken = ExtString::randomString(0, 0, 4, 0);
    t0003.verify_token = verifyToken;
    t0003.verify_token_valid_until = std::chrono::system_clock::now() + std::chrono::minutes(60);
    opi.insertObject(t0003, t0003.appuser_profile_id);
    return true;
}

bool DatabaseLogicAppUser::verifyAppUser(const reducedsole::uuid &appId,
                                         const std::string &loginEMail,
                                         const std::string &verifyToken,
                                         std::string &message,
                                         ExtRapidJSONWriter &w,
                                         reducedsole::uuid &appUserId)
{
    size_t count(0);
    t0003_appuser_profiles t0003;
    if (!opi.selectObject({{t0003.loginemail.name(), loginEMail},
                           {t0003.app_id.name(), appId.str()}},
                          t0003,
                          count))
    {
        message = std::string("no appuser with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found");
        return false;
    }
    if (count > 1)
    {
        message = std::string("more than one user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found. This is definitely a fatal error!");
        return false;
    }
    appUserId = t0003.appuser_profile_id;
    logStatController.log(__FILE__, __LINE__, LogStatController::verbose,
                          std::string("verify_token_valid_until as string: ") + t0003.verify_token_valid_until.asString());
    std::chrono::system_clock::time_point verify_token_valid_until(t0003.verify_token_valid_until);
    {
        std::time_t t = std::chrono::system_clock::to_time_t(verify_token_valid_until);
        std::stringstream ss;
        ss << std::put_time( std::localtime( &t ), "%FT%T%z" );
        logStatController.log(__FILE__, __LINE__, LogStatController::verbose,
                              std::string("verify_token_valid_until: ") + ss.str());
    }
    std::chrono::system_clock::time_point now(std::chrono::system_clock::now());
    {
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time( std::localtime( &t ), "%FT%T%z" );
        logStatController.log(__FILE__, __LINE__, LogStatController::verbose,
                              std::string("now: ") + ss.str());
    }
    if (verify_token_valid_until < now)
    {
        message = std::string("Token not valid any more, please request new Token.");
        return false;
    }
    if (t0003.verify_token != verifyToken)
    {
        message = std::string("wrong token");
        return false;
    }

    {
        t0003.verified = TimePointPostgreSqlNow;
        t0003.verify_token = "";
        t0003.verify_token_valid_until = TimePointPostgreSqlNull;
        opi.insertObject(t0003, t0003.appuser_profile_id);
    }
    std::string loginToken;
    loginSuccessful(appUserId, loginToken);
    w.addMember("loginToken", loginToken);
    w.addMember(t0003.appuser_profile_id.name(), t0003.appuser_profile_id.asString());
    w.addMember(t0003.fstname.name(), t0003.fstname);
    w.addMember(t0003.surname.name(), t0003.surname);
    w.addMember(t0003.visible_name.name(), t0003.visible_name);
    return true;
}

bool DatabaseLogicAppUser::loginAppUser(const reducedsole::uuid &appId,
                                        const std::string &loginEMail,
                                        const std::string &password,
                                        std::string &message,
                                        ExtRapidJSONWriter &w,
                                        reducedsole::uuid &appUserId)
{
    if (!lookupAppUser(appId, loginEMail, appUserId, message))
    {
        message = "LoginEMail/User not found. Please check your LoginEMail or register first.";
        return false;
    }
    PGExecutor login(pool);
    if (!login.login(tableNames.t0004_appuser_passwordhashes,
                     "password_hash",
                     password,
                     tableFields.appuser_id,
                     appUserId.str()))
    {
        message = "Password not set, please request a Logintoken";
        return false;

    }
    if (!login.boolean("login_ok"))
    {
        message = "Password is wrong";
        return false;
    }
    PGExecutor select(pool);
    if (!select.select(tableNames.t0003_appuser_profiles,
                       tableFields.id,
                       appUserId))
    {
        message = "Fatal error, could not load profile data";
        return false;
    }
    std::string loginToken;
    loginSuccessful(appUserId, loginToken);
    w.addMember("loginToken", loginToken);
    w.addMember(tableFields.id, select.string(tableFields.id));
    w.addMember(tableFields.fstname, select.string(tableFields.fstname));
    w.addMember(tableFields.surname, select.string(tableFields.surname));
    w.addMember(tableFields.visible_name, select.string(tableFields.visible_name));
    w.addMember(tableFields.image_id, select.string(tableFields.image_id));
    return true;
}

bool DatabaseLogicAppUser::updateAppUser(const reducedsole::uuid &appId,
                                         const reducedsole::uuid &userId,
                                         const std::string &fstname,
                                         const std::string &surname,
                                         const std::string &visible_name,
                                         const std::string &color,
                                         const std::string &message_font_color,
                                         const bool searching_exactly_allowed,
                                         const bool searching_fuzzy_allowed,
                                         const std::string &public_key_base64,
                                         const bool with_image,
                                         const reducedsole::uuid imageId,
                                         std::string &message)
{
    SqlString sql;
    sql.update(tableNames.t0003_appuser_profiles);
    sql.addSet(MACRO_NameValue(fstname), false);
    sql.addSet(MACRO_NameValue(surname), false);
    sql.addSet(MACRO_NameValue(visible_name), false);
    sql.addSet(MACRO_NameValue(color), false);
    sql.addSet(MACRO_NameValue(message_font_color), false);
    sql.addSet(MACRO_NameValue(searching_exactly_allowed), false);
    sql.addSet(MACRO_NameValue(searching_fuzzy_allowed), false);
    sql.addSet(MACRO_NameValue(public_key_base64), false);
    if (with_image)
    {
        sql.addSet(tableFields.image_id, imageId, false);
    }
    sql.addCompare("where", tableFields.app_id, "=", appId);
    sql.addCompare("and", tableFields.id, "=", userId);
    PGExecutor e(pool, sql);
    message = "profile updated";
    return true;
}

bool DatabaseLogicAppUser::appUserLoggedIn(const reducedsole::uuid &appId,
                                           const std::string &loginEMail,
                                           const std::string &loginToken,
                                           reducedsole::uuid &userId,
                                           std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::string ignored;
    if (!lookupAppUser(appId, loginEMail, userId, ignored))
    {
        return false;
    }
    SqlString sql("select * from ");
    sql += tableNames.t0009_appuser_logintoken;
    sql.addCompare("where", "appuser_id", "=", userId);
    sql.addCompare("and", "login_token", "=", loginToken);
    PGExecutor e(pool, sql);
    if (e.size() == 0)
    {
        return false;
    }
    loginTokenValidUntil = e.timepoint("login_token_valid_until");
    return true;
}

bool DatabaseLogicAppUser::requestUpdatePassword(const reducedsole::uuid &appId,
                                                 const std::string &loginEMail,
                                                 std::string &updatePasswordToken,
                                                 std::string &message)
{
    reducedsole::uuid userId;
    if (!lookupAppUser(appId, loginEMail, userId, message))
    {
        return false;
    }
    updatePasswordToken = ExtString::randomString(0, 0, 4, 0);
    SqlString sql;
    sql.update(tableNames.t0003_appuser_profiles);
    sql.addSet("update_password_token", updatePasswordToken, false);
    sql.addSet("update_password_token_valid_until", std::chrono::system_clock::now() + std::chrono::minutes(60), false);
    sql.addCompare("where", "id", "=", userId);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicAppUser::updatePasswordLoggedIn(const reducedsole::uuid &appuser_id, const std::string &password)
{
    SqlString sql;
    sql.select(tableNames.t0004_appuser_passwordhashes);
    sql.addCompare("where", tableFields.appuser_id, "=", appuser_id);
    PGExecutor e(pool, sql);
    if (e.size())
    {
        SqlString sql;
        sql.update(tableNames.t0004_appuser_passwordhashes);
        sql += " password_hash = crypt(:password, gen_salt('bf')) ";
        sql.set("password", password);
        sql.addCompare("where", "appuser_id", "=", appuser_id);
        PGExecutor e(pool, sql);
    }
    else
    {
        SqlString sql("insert into t0004_appuser_passwordhashes "
                      " (id, appuser_id, password_hash) "
                      " values "
                      " (:id, :appuser_id, crypt(:password, gen_salt('bf'))) ");
        sql.set(tableFields.id, reducedsole::uuid4());
        sql.set(tableFields.appuser_id, appuser_id);
        sql.set("password", password);
        PGExecutor e(pool, sql);

    }
    return true;
}

bool DatabaseLogicAppUser::updatePassword(const reducedsole::uuid &appId,
                                          const std::string &loginEMail,
                                          const std::string &updatePasswordToken,
                                          const std::string &password,
                                          std::string &message,
                                          std::string &loginToken,
                                          reducedsole::uuid &userId)
{
    PGExecutor e(pool);
    e.select(tableNames.t0003_appuser_profiles,
             "loginemail",
             loginEMail,
             "app_id",
             appId.str(),
             "deleted",
             TimePointPostgreSqlNull);
    if (e.size() == 0)
    {
        message = std::string("no appuser with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found");
        return false;
    }
    if (e.size() > 1)
    {
        message = std::string("more than one user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found. This is definitely a fatal error!");
        return false;
    }
    if (e.isNull("update_password_token_valid_until"))
    {
        message = std::string("not update password token requested");
        return false;
    }
    userId = e.uuid("id");
    std::chrono::system_clock::time_point update_password_token_valid_until(e.timepoint("update_password_token_valid_until"));
    std::chrono::system_clock::time_point now(std::chrono::system_clock::now());
    if (update_password_token_valid_until < now)
    {
        message = std::string("update password token not valid any more, please init update password again.");
        resetUpdatePasswordToken(userId);
        return false;
    }
    if (e.string("update_password_token") != updatePasswordToken)
    {
        message = std::string("wrong updatePasswordToken");
        return false;
    }
    resetUpdatePasswordToken(userId);

    {
        SqlString sql;
        sql.update(tableNames.t0004_appuser_passwordhashes);
        sql += " password_hash = crypt(:password, gen_salt('bf')) ";
        sql.set("password", password);
        sql.addCompare("where", "appuser_id", "=", userId);
        PGExecutor e(pool, sql);
    }
    {
        PGExecutor erase(pool);
        erase.delet(tableNames.t0009_appuser_logintoken, "appuser_id", userId.str());
    }
    loginSuccessful(userId, loginToken);
    message = "update password successful";
    return true;

}

bool DatabaseLogicAppUser::searchProfiles(const reducedsole::uuid &appId,
                                          const std::string &needle,
                                          size_t limit,
                                          const size_t offset,
                                          std::string &message,
                                          rapidjson::Value &target,
                                          rapidjson::MemoryPoolAllocator<> &alloc)
{
    if (!needle.size())
    {
        message = "you must provide a needle to search for";
        return false;
    }
    if (limit == 0)
    {
        limit = 50;
    }
    std::vector<std::string> needles;
    ExtString::split(needle, " ", needles);
    SqlString sql("select id, visible_name, image_id from ");
    sql += tableNames.t0003_appuser_profiles;
    sql.addCompare("where", "app_id", "=", appId);
    sql.addCompare("and", "verified", "is not", TimePointPostgreSqlNull);
    sql += " and ( ";
    sql += " ( searching_fuzzy_allowed ";
    int index(0);
    for (const auto &n : needles)
    {
        std::string variable(std::string("v") + ExtString::toString(index));
        sql += std::string(" and lower(visible_name) like lower(:") + variable + ") ";
        sql.set(variable, std::string("%") + n + std::string("%"));
        ++index;
    }
    sql += " ) or ( ";
    sql += " searching_exactly_allowed and visible_name = :needle ";
    sql.set("needle", needle);
    sql += " ) ) ";
    sql.limit(limit, offset);
    ORM2Postgres o2p(pool);
    o2p.toJsonArray(sql, target, alloc);
    return true;
}

bool DatabaseLogicAppUser::fetchMyProfile(const reducedsole::uuid &appId,
                                          const reducedsole::uuid &userId,
                                          std::string &message,
                                          rapidjson::Value &target,
                                          rapidjson::MemoryPoolAllocator<> &alloc)
{
    PGExecutor e(pool);
    e.select(tableNames.t0003_appuser_profiles,
             tableFields.app_id, appId.str(),
             tableFields.id, userId.str(),
             tableFields.deleted, TimePointPostgreSqlNull);
    if (!e.size())
    {
        message = "could not find user with id: " + userId.str();
        return false;
    }
    target.SetObject();
    target.AddMember(rapidjson::StringRef(tableFields.message_font_color), e.string(tableFields.message_font_color), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.color), e.string(tableFields.color), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.fstname), e.string(tableFields.fstname), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.surname), e.string(tableFields.surname), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.visible_name), e.string(tableFields.visible_name), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.image_id), e.string(tableFields.image_id), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.searching_exactly_allowed), e.boolean(tableFields.searching_exactly_allowed), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.searching_fuzzy_allowed), e.boolean(tableFields.searching_fuzzy_allowed), alloc);
    return true;
}

bool DatabaseLogicAppUser::fetchProfile(const reducedsole::uuid &appId,
                                        const reducedsole::uuid &userId,
                                        std::string &message,
                                        rapidjson::Value &target,
                                        rapidjson::MemoryPoolAllocator<> &alloc)
{
    PGExecutor e(pool);
    e.select(tableNames.t0003_appuser_profiles,
             tableFields.app_id, appId.str(),
             tableFields.id, userId.str(),
             tableFields.deleted, TimePointPostgreSqlNull);
    if (!e.size())
    {
        message = "could not find user with id: " + userId.str();
        return false;
    }
    target.SetObject();
    target.AddMember("id", e.string("id"), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.message_font_color), e.string(tableFields.message_font_color), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.color), e.string(tableFields.color), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.visible_name), e.string(tableFields.visible_name), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.public_key_base64), e.string(tableFields.public_key_base64), alloc);
    target.AddMember(rapidjson::StringRef(tableFields.image_id), e.string(tableFields.image_id), alloc);
    return true;
}

bool DatabaseLogicAppUser::storeDeviceToken(const reducedsole::uuid &userId,
                                            const std::string &device_token)
{
    SqlString sql;
    sql.insert(tableNames.t0015_appuser_devicetoken);
    sql.addInsert(tableFields.id, reducedsole::uuid4(), false);
    sql.addInsert(tableFields.user_id, userId, false);
    sql.addInsert(tableFields.device_token, device_token, false);
    PGExecutor e(pool,
                 sql);
    return true;
}

bool DatabaseLogicAppUser::removeDeviceToken(const reducedsole::uuid &userId,
                                             const std::string &device_token)
{
    SqlString sql;
    sql.delet(tableNames.t0015_appuser_devicetoken);
    sql.addCompare(" where ", tableFields.user_id, " = ", userId);
    sql.addCompare(" and ", tableFields.device_token, " = ", device_token);
    PGExecutor e(pool,
                 sql);
    return true;
}

size_t DatabaseLogicAppUser::fetchDeviceToken(const reducedsole::uuid &userId,
                                              std::set<std::string> &device_token)
{
    SqlString sql;
    sql.select(tableNames.t0015_appuser_devicetoken);
    sql.addCompare(" where ", tableFields.user_id, " = ", userId);
    PGExecutor e(pool,
                 sql);
    for (size_t i(0); i < e.size(); ++i)
    {
        device_token.insert(e.string(tableFields.device_token));
        e.next();
    }
    return e.size();
}

void DatabaseLogicAppUser::refreshAppUserLoginToken(const reducedsole::uuid &appId,
                                                    const std::string &loginEMail,
                                                    std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::string ignored;
    reducedsole::uuid appUserId;
    if (!lookupAppUser(appId, loginEMail, appUserId, ignored))
    {
        return;
    }

    SqlString sql("update ");
    sql += tableNames.t0009_appuser_logintoken;
    sql += " set login_token_valid_until = now() + interval '1 hour' *:validhours "
           "where appuser_id = :appuserid "
           "returning login_token_valid_until";
    MACRO_set(sql, appUserId);
    int validHours(24 * 7);
    MACRO_set(sql, validHours);
    PGExecutor e(pool, sql);
    loginTokenValidUntil = e.timepoint(tableFields.login_token_valid_until);
}

void DatabaseLogicAppUser::resetUpdatePasswordToken(const reducedsole::uuid &userId)
{
    SqlString sql;
    sql.update(tableNames.t0003_appuser_profiles);
    sql.addSet("update_password_token", "", false);
    sql.addSet("update_password_token_valid_until", TimePointPostgreSqlNull, true);
    sql.addCompare("where", "id", "=", userId);
    PGExecutor e(pool, sql);

}

