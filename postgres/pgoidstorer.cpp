#include "pgoidstorer.h"
#include "pgconnection.h"

void PGOidStorer::exec(PGConnectionPool &pool,
                       const std::basic_string<std::byte> &data,
                       pqxx::oid &result)
{
    PGConnection conn(pool);
    pqxx::work w(*conn.getConnection());
    result = pqxx::blob::from_buf(w, data);
    w.commit();
}

PGOidStorer::PGOidStorer(PGConnectionPool &pool,
                         const std::basic_string<std::byte> &data,
                         pqxx::oid &result)
{
    exec(pool, data, result);
}

PGOidStorer::PGOidStorer(PGConnectionPool &pool,
                         const std::string &data,
                         pqxx::oid &result)
{
    exec(pool,
         std::basic_string<std::byte>(reinterpret_cast<const std::byte*>(data.data()), data.size()),
         result);
}
