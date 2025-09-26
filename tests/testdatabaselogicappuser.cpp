#include "testdatabaselogicappuser.h"

#include "thirds/doctest/doctest/doctest.h"

TestDatabaseLogicAppUser::TestDatabaseLogicAppUser(DatabaseLogicAppUser &databaseLogicAppUser)
{
    reducedsole::uuid appId(reducedsole::rebuild("6ce520ba-4c44-47be-9386-15f021ea3a41"));
//    reducedsole::uuid u(databaseLogicAppUser.getUserId(appId, "jens@wienoebst.com"));
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
    reducedsole::uuid userId;
    databaseLogicAppUser.updatePassword(appId,
                                        loginEMail,
                                        updatePasswordToken,
                                        "1234",
                                        message,
                                        loginToken,
                                        userId);*/
    {
        reducedsole::uuid userId(reducedsole::uuid4());
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

