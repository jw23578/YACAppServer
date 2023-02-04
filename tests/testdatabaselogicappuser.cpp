#include "testdatabaselogicappuser.h"

#include "thirds/doctest/doctest/doctest.h"

TestDatabaseLogicAppUser::TestDatabaseLogicAppUser(DatabaseLogicAppUser &databaseLogicAppUser)
{
    sole::uuid appId(sole::rebuild("6ce520ba-4c44-47be-9386-15f021ea3a41"));
    sole::uuid u(databaseLogicAppUser.getAppUserId(appId, "jens@wienoebst.com"));
    /*
    std::string loginEMail("jens@wienoebst.com");
    std::string updatePasswordToken;
    std::string message;
    databaseLogicAppUser.requestUpdatePassword(appId,
                                            loginEMail,
                                            updatePasswordToken,
                                            message);
    if (updatePasswordToken.size() == 0)
    {
        return;
    }
    std::string loginToken;
    sole::uuid userId;
    databaseLogicAppUser.updatePassword(appId,
                                        loginEMail,
                                        updatePasswordToken,
                                        "1234",
                                        message,
                                        loginToken,
                                        userId);*/
    {
        sole::uuid userId(sole::uuid4());
        std::string device_token("irgendein text");
        databaseLogicAppUser.storeDeviceToken(userId, device_token);

        std::set<std::string> all_device_token;
        databaseLogicAppUser.fetchDeviceToken(userId, all_device_token);
        if (all_device_token.size() != 1)
        {
            error = true;
        }

        databaseLogicAppUser.removeDeviceToken(userId, device_token);

        all_device_token.clear();
        databaseLogicAppUser.fetchDeviceToken(userId, all_device_token);
        if (all_device_token.size() != 0)
        {
            error = true;
        }
    }
}

