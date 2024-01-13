#ifndef DATABASELOGICTHIRDPARTY_H
#define DATABASELOGICTHIRDPARTY_H

#include "databaselogicinterface.h"
#include "orm_implementions/t0029_third_party_user_data.h"

class DatabaseLogicThirdParty: public DatabaseLogicInterface
{
public:
    DatabaseLogicThirdParty(LogStatController &logStatController,
                            PGConnectionPool &pool);

    bool create(const sole::uuid &app_id,
                const std::string &third,
                const std::string &mandant,
                const std::string &user_id_string,
                const std::string &image_id_string,
                const sole::uuid &appuser_id,
                t0029_third_party_user_data &result);

    bool update(const t0029_third_party_user_data &user);

    bool lookup(const sole::uuid app_id,
                const std::string &third,
                const std::string &mandant,
                const std::string &user_id_string,
                t0029_third_party_user_data &result);
};

#endif // DATABASELOGICTHIRDPARTY_H
