#include "handler_t0030_documents.h"
#include "orm_implementions/t0030_documents.h"
#include "base64.h"

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
    if (isPost())
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

        opi.insertObject(t0030);

        answerOk("Document stored", true);
        return;
    }
    if (isGet())
    {
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
