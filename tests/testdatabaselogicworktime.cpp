#include "testdatabaselogicworktime.h"

TestDatabaseLogicWorktime::TestDatabaseLogicWorktime(LogStatController &logStatController,
                                                     DatabaseLogics &databaseLogics):
    logStatController(logStatController),
    databaseLogics(databaseLogics)
{

    sole::uuid appId(sole::rebuild("6ce520ba-4c44-47be-9386-15f021ea3a41"));
    std::string loginEMail("jens@wienoebst.com");
    sole::uuid userId(databaseLogics.databaseLogicAppUser.getUserId(appId, loginEMail));
    DatabaseLogicWorktime::UserMood userMood(DatabaseLogicWorktime::UserMoodUnknown);
    DatabaseLogicWorktime::DayRating dayRating(DatabaseLogicWorktime::DayRatingUnknown);
    std::chrono::system_clock::time_point workStart;
    std::chrono::system_clock::time_point pauseStart;
    std::chrono::system_clock::time_point offSiteWorkStart;
    if (!databaseLogics.databaseLogicWorktime.currentState(userId, workStart, pauseStart, offSiteWorkStart))
    {
        return;
    }
    std::string message;
    databaseLogics.databaseLogicWorktime.insertWorktime(userId, std::chrono::system_clock::now(), DatabaseLogicWorktime::OffSiteWorkEndType,
                                                        userMood,
                                                        dayRating,
                                                        workStart,
                                                        pauseStart,
                                                        offSiteWorkStart,
                                                        message);
    logStatController.log(__FILE__, __LINE__, LogStatController::info, message);

    message = "";
    databaseLogics.databaseLogicWorktime.insertWorktime(userId, std::chrono::system_clock::now(), DatabaseLogicWorktime::OffSiteWorkStartType,
                                                        userMood,
                                                        dayRating,
                                                        workStart,
                                                        pauseStart,
                                                        offSiteWorkStart,
                                                        message);
    logStatController.log(__FILE__, __LINE__, LogStatController::info, message);

    message = "";
    databaseLogics.databaseLogicWorktime.insertWorktime(userId, std::chrono::system_clock::now(), DatabaseLogicWorktime::WorkStartType,
                                                        userMood,
                                                        dayRating,
                                                        workStart,
                                                        pauseStart,
                                                        offSiteWorkStart,
                                                        message);
    logStatController.log(__FILE__, __LINE__, LogStatController::info, message);
}
