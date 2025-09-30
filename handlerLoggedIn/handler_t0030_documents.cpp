#include "handler_t0030_documents.h"
#include "orm_implementions/t0030_documents.h"
#include "base64.h"
#include "orm_implementions/t0031_catchphrases.h"
#include "orm_implementions/t0032_catchphrase2document.h"
#include "JWUtils/extstringview.h"
#include "coutlogger.h"

Handler_t0030_documents::Handler_t0030_documents(PistacheServerInterface &serverInterface,
                                                 LoggedInAppUsersContainer &loggedInAppUsersContainer,
                                                 PGORMPersistence &opi):
    HandlerLoggedInInterface(serverInterface, opi, loggedInAppUsersContainer)
{
    addAllMethodTypes(serverInterface, t0030_documents().getORMName());
}

void Handler_t0030_documents::method(CurrentContext &context)
{
    coutLogger::ActivateVisibleLogging avl;
    if (isPost() || isPut())
    {
        t0030_documents t0030;
        ORM2rapidjson orm2json;
        if (!orm2json.fromJson(getPostedData(), t0030))
        {
            answerOk("JSON-decoding of Object failed", false);
            return;
        }
        if (t0030.transfer_document_base64.size() == 0)
        {
            answerOk("No Document-Data sended", false);
            return;
        }        

        std::basic_string<std::byte> data;
        bin_base64_decode(t0030.transfer_document_base64, data);
        if (data.size() == 0)
        {
            answerOk("No decoded Document-Data", false);
            return;
        }
        t0030.document_blob_id = context.opi.storeBlob(data, context.userId);
        t0030.app_id = context.appId;
        t0030.creater_id = context.userId;

        if (isPost())
        {
            if (!context.opi.insertObject(t0030, context.userId))
            {
                answerOk("Insert of Document failed", false);
                return;
            }
        }
        if (isPut())
        {
            if (!context.opi.insertObject(t0030, context.userId))
            {
                answerOk("Update of Document failed", false);
                return;
            }
        }
        std::basic_string<std::byte> comma_separated_catchphrases;
        bin_base64_decode(t0030.transfer_comma_separated_catchphrases_base64, comma_separated_catchphrases);

        std::string_view work((char*)comma_separated_catchphrases.data(), comma_separated_catchphrases.size());
        std::set<std::string_view> catchprases;
        ExtStringView::split(work, ",", catchprases);

        for (const auto &cp : catchprases)
        {
            t0031_catchphrases t0031;
            t0031.id.generate();
            t0031.catchphrase = std::string(cp);
            t0031.app_id = context.appId;
            context.opi.insertIfNotSameDataExists(t0031, context.userId);

            t0032_catchphrase2document t0032;
            t0032.id.generate();
            t0032.app_id = context.appId;
            t0032.document_id = t0030.id;
            t0032.t0031_id = t0031.id;
            context.opi.insertObject(t0032, context.userId);
        }

        answerOk("Document stored", true);
        return;
    }
    if (isGet())
    {
        MACRO_GetString(needle);
        MACRO_GetInt(limit);
        MACRO_GetInt(offset);
        if (needle.size() || limit)
        {
            // dokumentsuche
            limit = std::min(100, limit);
            if (limit == 0)
            {
                limit = 100;
            }
            SqlString sql("set lc_time='de_DE.UTF-8'; select *, encode(comma_separated_catchphrases::bytea, 'base64') as transfer_comma_separated_catchphrases_base64 from "
                          "(select *, (select coalesce(string_agg(catchphrase, ', '), '') from t0031_catchphrases t0031 "
                          "where t0031.id in (select t0031_id from t0032_catchphrase2document t0032 where t0032.document_id = t0030.id)) as comma_separated_catchphrases from ");
            sql += tableNames.t0030_documents + " t0030 ";
            sql.addCompare("where", tableFields.creater_id, "=", context.userId);
            sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
            sql += ") docs ";
            sql.addCompare("where", "'1'", "=", "1");
            std::set<std::string> needles;
            ExtString::split(needle, " ", needles);
            for (auto &n: needles)
            {
                sql.addCompare("and", "lower(document_name || '#' || document_type || '#' || comma_separated_catchphrases || '#' || to_char(created_datetime, 'TMday#TMmonth#YYYY'))", "like", std::string("%") + ExtString::lower(n) + "%");
            }
            sql += std::string(" order by created_datetime ");
            sql.limit(limit, offset);
            ORMVector<t0030_documents> documents;
            if (!context.opi.fetchObjects(sql, documents))
            {
                answerOk("No matching Document found", true);
                return;
            }
            rapidjson::Document d;
            d.SetObject();
            d.AddMember("message", rapidjson::StringRef("found documents"), d.GetAllocator());

            rapidjson::Document jsonDocuments;
            jsonDocuments.SetArray();
            for (size_t i(0); i < documents.size(); ++i)
            {
                const t0030_documents &t0030(documents[i]);
                rapidjson::Document jsonDoc;
                jsonDoc.SetObject();
                ExtRapidJSONWriter writer(jsonDoc, d.GetAllocator());
                writer.addMember("ORMName", t0030.getORMName());
                ORM2rapidjson orm2json;
                orm2json.add(writer, t0030.id);
                orm2json.add(writer, t0030.document_id);
                orm2json.add(writer, t0030.document_version);
                orm2json.add(writer, t0030.document_name);
                orm2json.add(writer, t0030.document_type);
                orm2json.add(writer, t0030.document_description);
                orm2json.add(writer, t0030.created_datetime);
                orm2json.add(writer, t0030.document_size);
                orm2json.add(writer, t0030.transfer_comma_separated_catchphrases_base64);
                jsonDocuments.PushBack(jsonDoc, d.GetAllocator());
            }
            d.AddMember("documents", jsonDocuments, d.GetAllocator());
            answerOk(true, d);
//            answerOk("dokumente gefunden", true);
            return;
        }

        MACRO_GetMandatoryUuid(id);
        t0030_documents t0030;
        if (!context.opi.selectObject(id, t0030))
        {
            answerOk(std::string("No Document with id ") + id.pretty() + " found", false);
            return;
        }
        if (t0030.creater_id != context.userId)
        {
            answerOk(std::string("Not allowed to download this document, wrong Creator"), false);
            return;
        }
        if (!t0030.deleted_datetime.isNull())
        {
            answerOk(std::string("No Document with id ") + id.pretty() + " found", false);
            return;
        }
        std::basic_string<std::byte> data;
        if (!context.opi.fetchBlob(t0030.document_blob_id, data))
        {
            answerOk(std::string("Could not load Documentdata for id ") + t0030.document_blob_id.asString(), false);
            return;
        }
        t0030.transfer_document_base64 = base64_encode(data);
        rapidjson::Document document;
        ORM2rapidjson orm2json;
        orm2json.toJson(t0030, document);
        answerOk(true, document);
        return;
    }
    if (isDelete())
    {
        MACRO_GetMandatoryUuid(id);
        t0030_documents t0030;
        if (!context.opi.selectObject(id, t0030))
        {
            answerOk(std::string("No Document with id ") + id.pretty() + " found", false);
            return;
        }
        if (t0030.creater_id != context.userId)
        {
            answerOk(std::string("Not allowed to delete this document, wrong Creator"), false);
            return;
        }
        context.opi.deleteObject(t0030, context.userId);
        answerOk("document deleted", true);
        return;
    }
    if (isPut())
    {

    }
}
