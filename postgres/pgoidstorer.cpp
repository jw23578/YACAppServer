#include "pgoidstorer.h"
#include "pgconnection.h"

PGOidStorer::PGOidStorer(PGConnectionPool &pool,
                         const std::basic_string<std::byte> &data,
                         pqxx::oid &result)
{
    PGConnection conn(pool);
    pqxx::work w(*conn.getConnection());
    result = pqxx::blob::from_buf(w, data);
    w.commit();
}

