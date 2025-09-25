#include "databaselogicuserandapp.h"
#include "sqlstring.h"
#include "pgexecutor.h"
#include "JWUtils/extstring.h"
#include "extrapidjson.h"
#include "base64.h"
#include "pgutils.h"
#include "pgoidstorer.h"
#include "pgoidloader.h"
#include "orm-mapper/orm2postgres.h"
#include "orm_implementions/t0027_app_images.h"

bool DatabaseLogicUserAndApp::lookUpOid(const reducedsole::uuid &id,
                                        pqxx::oid &imageOid)
{
    ORM2Postgres orm2postgres(pool);
    t0027_app_images t0027;
    if (!orm2postgres.select(id, t0027))
    {
        return false;
    }
    imageOid = t0027.image_oid;
    return true;
}

DatabaseLogicUserAndApp::DatabaseLogicUserAndApp(LogStatController &logStatController,
                                                 PGConnectionPool &pool,
                                                 ORMPersistenceInterface &opi):
    logStatController(logStatController),
    pool(pool),
    utils(pool),
    opi(opi)
{

}

bool DatabaseLogicUserAndApp::userIsAppOwner(const reducedsole::uuid &app_id,
                                             const reducedsole::uuid &user_id,
                                             std::string &errorMessage,
                                             bool &appExists)
{
    // FIXME
    return true;
/*    PGUtils utils(pool);
    t0002_apps t0002;
    SqlString sql(utils.createEntryExistsString(t0002.getORMName(), tableFields.id));
    sql.set(tableFields.id, app_id);
    PGExecutor e(pool, sql);
    appExists = e.size() > 0;
    if (e.size())
    {
        if (e.uuid("owner_id") != user_id)
        {
            errorMessage = "user is not app owner";
            return false;
        }
    }
    return true; */
}


bool DatabaseLogicUserAndApp::saveApp(const reducedsole::uuid loggedInUserId,
                                      t0001_apps &app,
                                      const std::string &installation_code,
                                      std::string &message)
{
    bool appExists(false);
    if (!userIsAppOwner(app.id, loggedInUserId, message, appExists))
    {
        return false;
    }

    pqxx::oid oid;
    PGOidStorer storeOid(pool, app.transfer_yacpck_base64, oid);
    app.yacpck_base64 = oid;
    app.owner_id = loggedInUserId;
    ORM2Postgres orm2postgres(pool);
    if (appExists)
    {
        orm2postgres.update(app);
        message = "sql error on updating the app";
        return false;
    }
    else
    {
        if (!orm2postgres.insert(app))
        {
            message = "sql error on inserting the app";
            return false;
        }
    }
    if (installation_code == "")
    {
        SqlString sql("update t0001_apps set "
                      "installation_code_hash = '' ");
        sql.addCompare("where", tableFields.id, "=", app.id);
        PGExecutor e(pool, sql);
    }
    else
    {
        SqlString sql("update t0001_apps set "
                      "installation_code_hash = crypt(:installation_code, gen_salt('bf')) ");
        sql.addCompare("where", tableFields.id, "=", app.id);
        MACRO_set(sql, installation_code);
        PGExecutor e(pool, sql);
    }
    return true;
}

size_t DatabaseLogicUserAndApp::getAllAPPs(rapidjson::Document &target)
{
    auto &alloc(target.GetAllocator());
    SqlString sql("select id "
                  ", app_name "
                  ", app_version "
                  ", app_logo_url "
                  ", app_color_name "
                  ", app_info_url "
                  ", search_code "
                  ", installation_code_hash "
                  ", array(select id from t0027_app_images where app_id = t0001.id order by position) as app_images "
                  "from t0001_apps t0001 "
                  "order by app_name ");
    PGExecutor e(pool, sql);
    rapidjson::Value allAPPs(rapidjson::kArrayType);
    for (size_t r(0); r < e.size(); ++r)
    {
        rapidjson::Value appObject;
        appObject.SetObject();
        ExtRapidJSONWriter ao(appObject, alloc);
        ao.addMember("search_code", e.string("search_code"));
        ao.addMember("installation_code_needed", e.string("installation_code_hash").size() > 0);
        appObject.AddMember("app_info_url", e.string("app_info_url"), alloc);
        appObject.AddMember("app_id", e.string("id"), alloc);
        appObject.AddMember("app_name", e.string("app_name"), alloc);
        appObject.AddMember("app_version", e.integer("app_version"), alloc);
        appObject.AddMember("app_logo_url", e.string("app_logo_url"), alloc);
        appObject.AddMember("app_color_name", e.string("app_color_name"), alloc);
        std::set<std::string> app_images;
        e.array("app_images", app_images);
        rapidjson::Value json_app_images;
        json_app_images.SetArray();
        for (const auto &i: app_images)
        {
            rapidjson::Value v(i.c_str(), alloc);
            json_app_images.PushBack(v, alloc);
        }
        appObject.AddMember("app_images", json_app_images, alloc);
        allAPPs.PushBack(appObject, alloc);
        e.next();
    }
    target.SetObject();
    target.AddMember("allApps", allAPPs, alloc);
    return e.size();
}

bool DatabaseLogicUserAndApp::fetchOneApp(const std::string &app_id,
                                          const int current_installed_version,
                                          const std::string &installation_code,
                                          rapidjson::Document &target)
{
    target.SetObject();
    auto &alloc(target.GetAllocator());
    ExtRapidJSONWriter t(target, alloc);

    SqlString sql("select app_name "
                  ", app_version "
                  ", json_yacapp "
                  ", yacpck_base64 "
                  ", coalesce(installation_code_hash, '') = '' or installation_code_hash = crypt(:installation_code, installation_code_hash) as installation_ok "
                  "from t0001_apps "
                  "where id = :app_id ");
    MACRO_set(sql, app_id);
    MACRO_set(sql, installation_code);
    PGExecutor e(pool, sql);
    if (!e.size())
    {
        target.AddMember("message", "app not found", alloc);
        return false;
    }
    if (!e.boolean("installation_ok"))
    {
        t.addMember("message", "wrong installation code");
        return false;
    }
    if (e.integer("app_version") <= current_installed_version)
    {
        target.AddMember("message", "app version is up to date", alloc);
        return true;
    }
    target.AddMember("app_id", app_id, alloc);
    target.AddMember("app_name", e.string("app_name"), alloc);
    target.AddMember("message", "app found", alloc);
    target.AddMember("json_yacapp", e.string("json_yacapp"), alloc);
    std::string yacpck_base64;
    PGOidLoader loader(pool,
                       e.oid("yacpck_base64"),
                       yacpck_base64);
    target.AddMember("yacpck_base64", yacpck_base64, alloc);
    return true;
}

bool DatabaseLogicUserAndApp::storeAppImage(t0027_app_images &t0027)
{
    pqxx::oid oid;
    std::vector<char> data;
    bin_base64_decode(t0027.transfer_image_base64, data);
    std::basic_string<std::byte> image_data(reinterpret_cast<std::byte*>(data.data()), data.size());
    PGOidStorer storeOid(pool, image_data, oid);
    t0027.image_oid = oid;
    ORM2Postgres orm2postgres(pool);
    orm2postgres.insert(t0027);
    return true;
}

bool DatabaseLogicUserAndApp::getAppImage(const reducedsole::uuid &id,
                                          std::basic_string<std::byte> &data,
                                          std::string &errorMessage)
{
    auto it(imageId2oid.find(id));
    pqxx::oid imageOid(0);
    if (it == imageId2oid.end())
    {
        if (!lookUpOid(id, imageOid))
        {
            errorMessage = "could not look up image oid for imageid: " + ExtString::toString(id);
            return false;
        }
    }
    else
    {
        imageOid = it->second;
    }

    PGOidLoader loader(pool, imageOid, data);
    if (data.size() == 0)
    {
        errorMessage = "could not load image with oid: " + ExtString::toString(imageOid);
        return false;
    }
    return true;
}



