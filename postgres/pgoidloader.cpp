#include "pgoidloader.h"
#include "pgconnection.h"

PGOidLoader::PGOidLoader(PGConnectionPool &pool,
                         pqxx::oid const &oid,
                         std::basic_string<std::byte> &target)
{
    PGConnection conn(pool);
    pqxx::work w(*conn.getConnection());
    std::size_t maxRead(1024 * 1024);
    while (pqxx::blob::append_to_buf(w, oid, target.size(), target, maxRead)) {}
    w.commit();
}
