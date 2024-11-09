#include "handler_t0030_documents.h"
#include "orm_implementions/t0030_documents.h"
#include "base64.h"
#include "orm_implementions/t0031_catchphrases.h"
#include "utils/extstringview.h"

Handler_t0030_documents::Handler_t0030_documents(PistacheServerInterface &serverInterface,
                                                 LoggedInAppUsersContainer &loggedInAppUsersContainer,
                                                 ORMPersistenceInterface &opi):
    HandlerLoggedInInterface(serverInterface, loggedInAppUsersContainer),
    opi(opi)
{
    addAllMethodTypes(serverInterface, t0030_documents().getORMName());
}

void Handler_t0030_documents::method()
{
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
        t0030.document_blob_id = opi.storeBlob(data);
        t0030.app_id = appId;

        if (isPost())
        {
            if (!opi.insertObject(t0030))
            {
                answerOk("Insert of Document failed", false);
                return;
            }
        }
        if (isPut())
        {
            if (!opi.updateObject(t0030))
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
            t0031.app_id = appId;
            opi.insertIfNotSameDataExists(t0031);
        }

        answerOk("Document stored", true);
        return;
    }
    if (isGet())
    {
        MACRO_GetString(needle);
        if (needle.size())
        {
            // dokumentsuche
            std::set<std::string> needles;
            ExtString::split(needle, " ", needles);
            SqlString sql("select * from "
                          "t0030_documents");
            sql.addCompare("where", "'1'", "=", "1");
            for (auto &n: needles)
            {
                sql.addCompare("and", "lower(document_name)", "like", std::string("%") + ExtString::lower(n) + "%");
            }
            ORMVector<t0030_documents> documents;
            if (!opi.fetchObjects(sql, documents))
            {
                answerOk("No matching Document found", false);
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
                jsonDoc.AddMember("document_name", rapidjson::StringRef(t0030.document_name.asString()), d.GetAllocator());
                jsonDoc.AddMember("document_type", rapidjson::StringRef(t0030.document_type.asString()), d.GetAllocator());
                jsonDoc.AddMember("document_description", rapidjson::StringRef(t0030.document_description.asString()), d.GetAllocator());
/*                jsonDoc.AddMember("id", rapidjson::StringRef(t0030.id.asString()), d.GetAllocator());*/
                jsonDocuments.PushBack(jsonDoc, d.GetAllocator());
            }
            d.AddMember("documents", jsonDocuments, d.GetAllocator());
            answerOk(true, d);
//            answerOk("dokumente gefunden", true);
            return;
        }

        MACRO_GetMandatoryUuid(id);
        t0030_documents t0030;
        if (!opi.selectObject(id, t0030))
        {
            answerOk(std::string("No Document with id ") + id.pretty() + " found", false);
            return;
        }
        std::basic_string<std::byte> data;
        if (!opi.fetchBlob(t0030.document_blob_id, data))
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

    }
    if (isPut())
    {

    }
}
