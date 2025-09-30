#include "databaselogicappuser.h"
#include "pgexecutor.h"
#include "JWUtils/definitions.h"
#include "orm_implementions/t0004_user_logintoken.h"
#include "orm-mapper/orm2postgres.h"
#include "logstatcontroller.h"
#include "JWUtils/extuuid.h"
#include "orm_implementions/t0003_user_passwordhashes.h"


DatabaseLogicAppUser::DatabaseLogicAppUser(LogStatController &logStatController,
                                           PGConnectionPool &pool,
                                           ORMPersistenceInterface &opi):
    logStatController(logStatController),
    pool(pool),
    opi(opi)
{

}

bool DatabaseLogicAppUser::loginUser(CurrentContext &context,
                                        const std::string &loginEMail,
                                        const std::string &password,
                                        std::string &message,
                                        ExtRapidJSONWriter &w,
                                        reducedsole::uuid &userId)
{
    t0002_user user;
    if (!t0002_user::lookupUser(context, loginEMail, user, message))
    {
        message = "LoginEMail/User not found. Please check your LoginEMail or register first.";
        return false;
    }
    userId = user.user_id;
    PGExecutor login(pool);
    if (!login.login(t0003_user_passwordhashes().getORMName(),
                     "password_hash",
                     password,
                     t0003_user_passwordhashes().user_id.name(),
                     userId.str()))
    {
        message = "Password not set or wrong, please request a Logintoken";
        return false;

    }
    if (!login.boolean("login_ok"))
    {
        message = "Password is wrong";
        return false;
    }
    t0002_user userProfile;
    if (!opi.selectObject({{userProfile.user_id.name(), userId.str()}}, userProfile))
    {
        message = "Fatal error, could not load profile data";
        return false;
    }
    t0004_user_logintoken userLogintoken;
    userLogintoken.loginSuccessful(context, userId);
    w.addMember("loginToken", userLogintoken.login_token);
    w.addMember(userProfile.user_id.name(), userProfile.user_id.asString());
    w.addMember(userProfile.fstname.name(), userProfile.fstname);
    w.addMember(userProfile.surname.name(), userProfile.surname);
    w.addMember(userProfile.visible_name.name(), userProfile.visible_name);
    w.addMember(userProfile.image_id.name(), userProfile.image_id.asString());
    return true;
}

bool DatabaseLogicAppUser::logoutAppUserByLoginToken(const std::string lt)
{
    t0004_user_logintoken loginToken;
    if (!opi.selectObject({{"login_token", lt}}, loginToken))
    {
        return false;
    }
    return opi.deleteObject(loginToken, loginToken.user_id);
}

bool DatabaseLogicAppUser::appUserLoggedIn(CurrentContext &context,
                                           const std::string &loginEMail,
                                           const std::string &loginToken,
                                           reducedsole::uuid &userId,
                                           std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::string ignored;
    t0002_user user;
    if (!t0002_user::lookupUser(context, loginEMail, user, ignored))
    {
        return false;
    }
    userId = user.user_id;
    SqlString sql("select * from ");
    sql += t0004_user_logintoken().getORMName();
    sql.addCompare("where", t0004_user_logintoken().user_id.name(), "=", userId);
    sql.addCompare("and", t0004_user_logintoken().login_token.name(), "=", loginToken);
    PGExecutor e(pool, sql);
    if (e.size() == 0)
    {
        return false;
    }
    loginTokenValidUntil = e.timepoint("login_token_valid_until");
    return true;
}

bool DatabaseLogicAppUser::requestUpdatePassword(CurrentContext &context,
                                                 const std::string &loginEMail,
                                                 std::string &updatePasswordToken,
                                                 std::string &message)
{
    t0002_user userProfile;
    if (!t0002_user::lookupUser(context, loginEMail, userProfile, message))
    {
        return false;
    }
    userProfile.update_password_token = ExtString::randomString(0, 0, 4, 0);
    updatePasswordToken = userProfile.update_password_token;
    userProfile.update_password_token_valid_until = std::chrono::system_clock::now() + std::chrono::minutes(60);
    opi.insertObject(userProfile, userProfile.user_id);
    return true;
}

bool DatabaseLogicAppUser::updatePasswordLoggedIn(const reducedsole::uuid &user_id, const std::string &password)
{
    SqlString sql;
    sql.select(t0003_user_passwordhashes().getORMName());
    sql.addCompare("where", t0003_user_passwordhashes().user_id.name(), "=", user_id);
    PGExecutor e(pool, sql);
    if (e.size())
    {
        SqlString sql;
        sql.update(t0003_user_passwordhashes().getORMName());
        sql += " password_hash = crypt(:password, gen_salt('bf')) ";
        sql.set("password", password);
        sql.addCompare("where", t0003_user_passwordhashes().user_id.name(), "=", user_id);
        PGExecutor e(pool, sql);
    }
    else
    {
        SqlString sql("insert into t0003_user_passwordhashes "
                      " (id, user_id, password_hash) "
                      " values "
                      " (:id, :user_id, crypt(:password, gen_salt('bf'))) ");
        sql.set(tableFields.id, reducedsole::uuid4());
        sql.set(t0003_user_passwordhashes().user_id.name(), user_id);
        sql.set("password", password);
        PGExecutor e(pool, sql);

    }
    return true;
}

bool DatabaseLogicAppUser::updatePassword(CurrentContext &context,
                                          const std::string &loginEMail,
                                          const std::string &updatePasswordToken,
                                          const std::string &password,
                                          std::string &message,
                                          std::string &loginToken,
                                          reducedsole::uuid &userId)
{
    size_t count(0);
    t0002_user userProfile;
    if (!opi.selectObject({{userProfile.loginemail.name(), loginEMail},
                           {userProfile.app_id.name(), context.appId.str()}},
                          userProfile,
                          count))
    {
        message = std::string("no user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found");
        return false;
    }
    if (count > 1)
    {
        message = std::string("more than one user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found. This is definitely a fatal error!");
        return false;
    }
    if (userProfile.update_password_token_valid_until.isNull())
    {
        message = std::string("no update password token requested");
        return false;
    }
    std::chrono::system_clock::time_point update_password_token_valid_until(userProfile.update_password_token_valid_until);
    std::chrono::system_clock::time_point now(std::chrono::system_clock::now());
    if (update_password_token_valid_until < now)
    {
        message = std::string("update password token not valid any more, please init update password again.");
        userProfile.clearUpdatePasswordToken();
        opi.insertObject(userProfile, userProfile.user_id);
        return false;
    }
    if (userProfile.update_password_token != updatePasswordToken)
    {
        message = std::string("wrong updatePasswordToken");
        return false;
    }
    userProfile.clearUpdatePasswordToken();
    opi.insertObject(userProfile, userProfile.user_id);

    {
        SqlString sql;
        sql.update(t0003_user_passwordhashes().getORMName());
        sql += " password_hash = crypt(:password, gen_salt('bf')) ";
        sql.set("password", password);
        sql.addCompare("where", t0003_user_passwordhashes().user_id.name(), "=", userId);
        PGExecutor e(pool, sql);
    }
    {
        PGExecutor erase(pool);
        erase.delet(t0004_user_logintoken().getORMName(), t0004_user_logintoken().user_id.name(), userId.str());
    }
    t0004_user_logintoken userLogintoken;
    userLogintoken.loginSuccessful(context, userId);
    loginToken = userLogintoken.login_token;
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
    t0002_user userProfile;
    sql += userProfile.getORMName();
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
    e.select(t0002_user().getORMName(),
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
    e.select(t0002_user().getORMName(),
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

void DatabaseLogicAppUser::refreshAppUserLoginToken(CurrentContext &context,
                                                    const std::string &loginEMail,
                                                    std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::string ignored;
    t0002_user userProfile;
    if (!t0002_user::lookupUser(context, loginEMail, userProfile, ignored))
    {
        return;
    }

    SqlString sql("update ");
    sql += t0004_user_logintoken().getORMName();
    sql += " set login_token_valid_until = now() + interval '1 hour' *:validhours "
           "where user_id = :userid "
           "returning login_token_valid_until";
    MACRO_set(sql, userProfile.user_id);
    int validHours(24 * 7);
    MACRO_set(sql, validHours);
    PGExecutor e(pool, sql);
    loginTokenValidUntil = e.timepoint(tableFields.login_token_valid_until);
}

