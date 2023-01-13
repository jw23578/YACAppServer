#include "databaselogicappuser.h"
#include "postgres/pgexecutor.h"
#include "definitions.h"

void DatabaseLogicAppUser::loginSuccessful(const sole::uuid &appUserId,
                                           std::string &loginToken)
{
    if (loginToken.size() == 0)
    {
        loginToken = sole::uuid4().str();
    }
    int validHours(24 * 7);
    PGSqlString sql(utils.createInsertString(tableNames.t0009_appuser_logintoken));
    sql.set("id", sole::uuid4());
    sql.set("appuser_id", appUserId);
    sql.set("login_token", loginToken);
    sql.set("login_token_valid_until", std::chrono::system_clock::now() + std::chrono::hours(1) * validHours);
    PGExecutor e(pool, sql);
}

bool DatabaseLogicAppUser::lookupAppUser(const sole::uuid &appId,
                                         const std::string &loginEMail,
                                         sole::uuid &appUserId,
                                         std::string &message)
{
    auto it(loginEMailAndAppId2AppUserId.find(loginEMail + appId.str()));
    if (it != loginEMailAndAppId2AppUserId.end())
    {
        appUserId = it->second;
        return true;
    }
    std::map<std::string, sole::uuid> loginEMail2AppUserId;
    PGExecutor appUser(pool);
    if (!appUser.select(tableNames.t0003_appuser_profiles,
                        "loginemail",
                        loginEMail,
                        "app_id",
                        appId.str(),
                        "deleted",
                        TimePointPostgreSqlNull))
    {
        message = "LoginEMail/User not found. Please check your LoginEMail or register first.";
        return false;
    }
    if (appUser.size() > 1)
    {
        message = std::string("more than one user with loginEMail: ") + ExtString::quote(loginEMail) + std::string(" found. This is definitely a fatal error!");
        return false;
    }
    if (appUser.isNull("verified"))
    {
        message = "LoginEMail/User not yet verified";
        return false;
    }
    loginEMail2AppUserId[loginEMail + appId.str()] = appUserId = appUser.uuid("id");
    return true;
}

DatabaseLogicAppUser::DatabaseLogicAppUser(LogStatController &logStatController,
                                           PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool),
    utils(pool)
{

}

sole::uuid DatabaseLogicAppUser::getUserId(const sole::uuid &appId,
                                           const std::string &loginEMail)
{
    PGSqlString sql("select id from ");
    sql += tableNames.t0001_users;
    sql.addCompare("where", "loginemail", "=", loginEMail);
    PGExecutor e(pool, sql);
    if (e.size() == 0)
    {
        return sole::uuid0();
    }
    return e.uuid("id");
}

bool DatabaseLogicAppUser::appUserExists(const sole::uuid &appId,
                                         const std::string &loginEMail)
{
    return utils.entryExists(tableNames.t0003_appuser_profiles,
                             "loginemail", loginEMail,
                             "app_id",
                             appId.str(),
                             "deleted",
                             TimePointPostgreSqlNull);
}

bool DatabaseLogicAppUser::createAppUser(sole::uuid const &appId,
                                         const std::string &loginEMail,
                                         const std::string &password,
                                         std::string &message,
                                         std::string &verifyToken)
{
    if (!utils.entryExists(tableNames.t0002_apps, "id", appId.str()))
    {
        message = "App with id: " + appId.str() + " does not exist";
        return false;
    }
    sole::uuid appUserId(sole::uuid4());
    verifyToken = ExtString::randomString(0, 0, 4, 0);
    {
        PGSqlString sql(utils.createInsertString(tableNames.t0003_appuser_profiles));
        sql.set("id", appUserId);
        sql.set("app_id", appId);
        sql.set("fstname", "");
        sql.set("surname", "");
        sql.set("visible_name", "");
        sql.set("verified", TimePointPostgreSqlNull);
        sql.set("loginemail", loginEMail);
        sql.set("verify_token", verifyToken);
        sql.set("verify_token_valid_until", std::chrono::system_clock::now() + std::chrono::minutes(60));
        sql.set("update_password_token", "");
        sql.set("update_password_token_valid_until", TimePointPostgreSqlNull);
        sql.set("deleted", TimePointPostgreSqlNull);
        PGExecutor e(pool, sql);
    }

    {
        PGSqlString sql("insert into t0004_appuser_passwordhashes "
                        " (id, appuser_id, password_hash) "
                        " values "
                        " (:id, :appuser_id, crypt(:password, gen_salt('bf'))) ");
        sql.set("id", sole::uuid4());
        sql.set("appuser_id", appUserId);
        sql.set("password", password);
        PGExecutor e(pool, sql);
    }

    return true;
}

bool DatabaseLogicAppUser::verifyAppUser(const sole::uuid &appId,
                                         const std::string &loginEMail,
                                         const std::string &verifyToken,
                                         std::string &message,
                                         std::string &loginToken)
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
    if (!e.isNull("verified"))
    {
        message = ExtString::quote(loginEMail) + " already verified, please login.";
        return false;
    }
    sole::uuid id(e.uuid("id"));
    logStatController.log(__FILE__, __LINE__, LogStatController::verbose,
                          std::string("verify_token_valid_until as string: ") + e.string("verify_token_valid_until"));
    std::chrono::system_clock::time_point verify_token_valid_until(e.timepoint("verify_token_valid_until"));
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
        message = std::string("verify token not valid any more, please register again");
        PGExecutor erase(pool);
        erase.erase(tableNames.t0003_appuser_profiles,
                    "id",
                    id.str());
        erase.erase(tableNames.t0004_appuser_passwordhashes,
                    "appuser_id",
                    id.str());
        return false;
    }
    if (e.string("verify_token") != verifyToken)
    {
        message = std::string("wrong verifyToken");
        return false;
    }

    {
        PGSqlString sql("update ");
        sql += tableNames.t0003_appuser_profiles;
        sql += " set verified = now(), "
               " verify_token = '', "
               " verify_token_valid_until = null "
               " where id = :id ";
        sql.set("id", id.str());
        PGExecutor e(pool, sql);
    }
    loginSuccessful(id, loginToken);
    return true;
}

bool DatabaseLogicAppUser::loginAppUser(const sole::uuid &appId,
                                        const std::string &loginEMail,
                                        const std::string &password,
                                        std::string &message,
                                        std::map<std::string, std::string> &data)
{
    sole::uuid appUserId;
    if (!lookupAppUser(appId, loginEMail, appUserId, message))
    {
        return false;
    }
    PGExecutor login(pool);
    if (!login.login(tableNames.t0004_appuser_passwordhashes,
                     "password_hash",
                     password,
                     "appuser_id",
                     appUserId.str()))
    {
        message = "LoginEMail/User not found. Please check your LoginEMail or register first.";
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
    data["loginToken"] = loginToken;
    data[tableFields.fstname] = select.string(tableFields.fstname);
    data[tableFields.surname] = select.string(tableFields.surname);
    data[tableFields.visible_name] = select.string(tableFields.visible_name);
    return true;
}

bool DatabaseLogicAppUser::updateAppUser(const sole::uuid &appId,
                                         const sole::uuid &userId,
                                         const std::string &fstname,
                                         const std::string &surname,
                                         const std::string &visible_name,
                                         const bool searching_exactly_allowed,
                                         const bool searching_fuzzy_allowed,
                                         const bool with_image,
                                         const sole::uuid imageId,
                                         std::string &message)
{
    PGSqlString sql;
    sql.update(tableNames.t0003_appuser_profiles);
    sql.addSet(MACRO_NameValue(fstname));
    sql.addSet(MACRO_NameValue(surname));
    sql.addSet(MACRO_NameValue(visible_name));
    sql.addSet(MACRO_NameValue(searching_exactly_allowed));
    sql.addSet(MACRO_NameValue(searching_fuzzy_allowed));
    if (with_image)
    {
        sql.addSet(tableFields.image_id, imageId);
    }
    sql.addCompare("where", tableFields.app_id, "=", appId);
    sql.addCompare("and", tableFields.id, "=", userId);
    PGExecutor e(pool, sql);
    message = "profile updated";
    return true;
}

bool DatabaseLogicAppUser::appUserLoggedIn(const sole::uuid &appId,
                                           const std::string &loginEMail,
                                           const std::string &loginToken,
                                           sole::uuid &userId,
                                           std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::string ignored;
    if (!lookupAppUser(appId, loginEMail, userId, ignored))
    {
        return false;
    }
    PGSqlString sql("select * from ");
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

bool DatabaseLogicAppUser::requestUpdatePassword(const sole::uuid &appId,
                                              const std::string &loginEMail,
                                              std::string &updatePasswordToken,
                                              std::string &message)
{
    sole::uuid userId;
    if (!lookupAppUser(appId, loginEMail, userId, message))
    {
        return false;
    }
    updatePasswordToken = ExtString::randomString(0, 0, 4, 0);
    PGSqlString sql;
    sql.update(tableNames.t0003_appuser_profiles);
    sql.addSet("update_password_token", updatePasswordToken);
    sql.addSet("update_password_token_valid_until", std::chrono::system_clock::now() + std::chrono::minutes(60));
    sql.addCompare("where", "id", "=", userId);
    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicAppUser::updatePassword(const sole::uuid &appId,
                                          const std::string &loginEMail,
                                          const std::string &updatePasswordToken,
                                          const std::string &password,
                                          std::string &message,
                                          std::string &loginToken,
                                          sole::uuid &userId)
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
        PGSqlString sql;
        sql.update(tableNames.t0004_appuser_passwordhashes);
        sql += " password_hash = crypt(:password, gen_salt('bf')) ";
        sql.set("password", password);
        sql.addCompare("where", "appuser_id", "=", userId);
        PGExecutor e(pool, sql);
    }
    {
        PGExecutor erase(pool);
        erase.erase(tableNames.t0009_appuser_logintoken, "appuser_id", userId.str());
    }
    loginSuccessful(userId, loginToken);
    message = "update password successful";
    return true;

}

bool DatabaseLogicAppUser::searchProfiles(const sole::uuid &appId,
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
    PGSqlString sql("select id, visible_name, image_id from ");
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
    PGExecutor e(pool, sql);
    e.toJsonArray(target, alloc);
    return true;
}

bool DatabaseLogicAppUser::fetchProfile(const sole::uuid &appId,
                                        const sole::uuid &userId,
                                        std::string &message,
                                        rapidjson::Value &target,
                                        rapidjson::MemoryPoolAllocator<> &alloc)
{
    PGExecutor e(pool);
    e.select(tableNames.t0003_appuser_profiles,
             "app_id", appId.str(),
             "id", userId.str(),
             "deleted", TimePointPostgreSqlNull);
    if (!e.size())
    {
        message = "could not find user with id: " + userId.str();
        return false;
    }
    target.SetObject();
    target.AddMember("id", e.string("id"), alloc);
    target.AddMember("visible_name", e.string("visible_name"), alloc);
    e.string("public_key_base64");
    target.AddMember("public_key_base64", "", alloc);
    target.AddMember("image_id", e.string("image_id"), alloc);
    return true;
}

bool DatabaseLogicAppUser::storeDeviceToken(const sole::uuid &userId,
                                            const std::string &device_token)
{
    PGSqlString sql;
    sql.insert(tableNames.t0015_appuser_devicetoken);
    sql.addInsert(tableFields.id, sole::uuid4());
    sql.addInsert(tableFields.user_id, userId);
    sql.addInsert(tableFields.device_token, device_token);
    PGExecutor e(pool,
                 sql);
    return true;
}

bool DatabaseLogicAppUser::removeDeviceToken(const sole::uuid &userId,
                                             const std::string &device_token)
{
    PGSqlString sql;
    sql.delet(tableNames.t0015_appuser_devicetoken);
    sql.addCompare(" where ", tableFields.user_id, " = ", userId);
    sql.addCompare(" and ", tableFields.device_token, " = ", device_token);
    PGExecutor e(pool,
                 sql);
    return true;
}

size_t DatabaseLogicAppUser::fetchDeviceToken(const sole::uuid &userId,
                                              std::set<std::string> &device_token)
{
    PGSqlString sql;
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

void DatabaseLogicAppUser::refreshAppUserLoginToken(const sole::uuid &appId,
                                                    const std::string &loginEMail,
                                                    std::chrono::system_clock::time_point &loginTokenValidUntil)
{
    std::string ignored;
    sole::uuid appUserId;
    if (!lookupAppUser(appId, loginEMail, appUserId, ignored))
    {
        return;
    }

    PGSqlString sql("update ");
    sql += tableNames.t0009_appuser_logintoken;
    sql += " set login_token_valid_until = now() + interval '1 hour' *:validhours "
           "where appuser_id = :appuserid "
           "returning login_token_valid_until";
    MACRO_set(appUserId);
    int validHours(24 * 7);
    MACRO_set(validHours);
    PGExecutor e(pool, sql);
    loginTokenValidUntil = e.timepoint("login_token_valid_until");
}

void DatabaseLogicAppUser::resetUpdatePasswordToken(const sole::uuid &userId)
{
    PGSqlString sql;
    sql.update(tableNames.t0003_appuser_profiles);
    sql.addSet("update_password_token", "");
    sql.addSet("update_password_token_valid_until", TimePointPostgreSqlNull);
    sql.addCompare("where", "id", "=", userId);
    PGExecutor e(pool, sql);

}

