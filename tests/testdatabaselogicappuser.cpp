#include "testdatabaselogicappuser.h"

TestDatabaseLogicAppUser::TestDatabaseLogicAppUser(DatabaseLogicAppUser &databaseLogicAppUser)
{
    sole::uuid appId(sole::rebuild("6ce520ba-4c44-47be-9386-15f021ea3a41"));
    std::string loginEMail("jens@wienoebst.com");
    std::string updatePasswordToken;
    std::string message;
    databaseLogicAppUser.requestUpdatePassword(appId,
                                            loginEMail,
                                            updatePasswordToken,
                                            message);
    if (updatePasswordToken.size() == 0)
    {
        exit(1);
    }
    std::string loginToken;
    sole::uuid userId;
    databaseLogicAppUser.updatePassword(appId,
                                        loginEMail,
                                        updatePasswordToken,
                                        "1234",
                                        message,
                                        loginToken,
                                        userId);
}
