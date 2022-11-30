#include "handlerappuserupdatepassword.h"
#include "extmap.h"

HandlerAppUserUpdatePassword::HandlerAppUserUpdatePassword(DatabaseLogicAppUser &databaseLogicAppUser,
                                                           LoggedInAppUsersContainer &loggedInAppUsersContainer,
                                                           PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "/updatePasswordAppUser",
                             TypePost,
                             TypeNoLoginNeeded),
    databaseLogicAppUser(databaseLogicAppUser),
    loggedInAppUsersContainer(loggedInAppUsersContainer)
{
    MACRO_GetMandatoryEMail(loginEMail);
    MACRO_GetMandatoryString(password);
    MACRO_GetMandatoryUuid(appId);
    MACRO_GetMandatoryString(updatePasswordToken);

    std::string message;
    std::string loginToken;
    sole::uuid userId;
    if (!databaseLogicAppUser.updatePassword(appId,
                                             loginEMail,
                                             updatePasswordToken,
                                             password,
                                             message,
                                             loginToken,
                                             userId))
    {
        answerBad(message);
        return;
    }
    loggedInAppUsersContainer.clear(userId);
    MACRO_CreateDataMAP(loginToken);
    answerOk(message, data);

}
