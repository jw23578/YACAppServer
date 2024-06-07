#include "handler_t0030_documents.h"
#include "orm_implementions/t0030_documents.h"
#include "base64.h"

Handler_t0030_documents::Handler_t0030_documents(PistacheServerInterface &serverInterface,
                                                 LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface, loggedInAppUsersContainer)
{
    addAllMethodTypes(serverInterface, t0030_documents().getORMName());
}

void Handler_t0030_documents::method()
{
    if (isPost())
    {
        t0030_documents t0030;
        ORM2rapidjson orm2json;
        orm2json.fromJson(getPostedData(), t0030);

        // pqxx::oid oid;
        // std::vector<char> data;
        // bin_base64_decode(t0030.transfer_document_base64, data);
        // std::basic_string<std::byte> image_data(reinterpret_cast<std::byte*>(data.data()), data.size());
        // PGOidStorer storeOid(pool, image_data, oid);
        // t0028.image_oid = oid;
        // ORM2Postgres orm2postgres(pool);
        // orm2postgres.insert(t0028);

    }
    if (isGet())
    {

    }
    if (isDelete())
    {

    }
    if (isPut())
    {

    }
}
