#include "pgoidstorer.h"
#include "pgconnection.h"

PGOidStorer::PGOidStorer(PGConnectionPool &pool,
                         const std::string &data,
                         pqxx::oid &result)
{
    PGConnection conn(pool);
    pqxx::work w(*conn.getConnection());
    const std::basic_string<std::byte> byteData(reinterpret_cast<const std::byte*>(&data[0]), data.size());
    result = pqxx::blob::from_buf(w, byteData);
    w.commit();
}
