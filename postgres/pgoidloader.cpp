#include "pgoidloader.h"
#include "pgconnection.h"

PGOidLoader::PGOidLoader(PGConnectionPool &pool,
                         pqxx::oid const &oid,
                         std::string &target)
{
    PGConnection conn(pool);
    pqxx::work w(*conn.getConnection());
    std::size_t maxRead(1024 * 1024);
    std::basic_string<std::byte> temp;
    while (pqxx::blob::append_to_buf(w, oid, temp.size(), temp, maxRead)) {}
    w.commit();
    target = std::string(reinterpret_cast<char*>(&temp[0]));
}
