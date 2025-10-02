#include "handlerstoremessage.h"
#include "curlWrapper/jw78firebasewrapper.h"
#include "rapidjson/writer.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm_implementions/t0007_messages.h"

HandlerStoreMessage::HandlerStoreMessage(const std::string &firebaseApiKey,
                                         DeviceTokenCache &deviceTokenCache,
                                         DatabaseLogics &databaseLogics,
                                         PistacheServerInterface &serverInterface,
                                         LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
                             "/storeMessage",
                             TypePost,
                             loggedInAppUsersContainer),
    firebaseApiKey(firebaseApiKey),
    deviceTokenCache(deviceTokenCache),
    databaseLogics(databaseLogics)
{
    addMethod(serverInterface, t0028.getORMName(), TypePost);
    addMethod(serverInterface, t0028.getORMName(), TypeGet);
}

void HandlerStoreMessage::method(CurrentContext &context)
{
    /*
    addMethod(serverInterface, t0007_messages().getORMName(), TypeDelete);
    if (isMethod(t0007_messages().getORMName()))
    {
        if (isDelete())
        {
            std::string resultMessage;
            MACRO_GetBool(allOfLoggedInUser);
            if (allOfLoggedInUser)
            {
                answerOk(resultMessage, databaseLogics.databaseLogicMessages.markAllOfUserMessageDeleted(loggedInUserId, resultMessage));
            }
            else
            {
                MACRO_GetMandatoryUuid(id);
                answerOk(resultMessage, databaseLogics.databaseLogicMessages.markMessageDeleted(id, loggedInUserId, resultMessage));
            }
        }
        return;
    }*/
    if (isMethod(t0028.getORMName()))
    {
        ORM2rapidjson orm2json;
        orm2json.fromJson(getPostedData(), t0028);
        if (isPost())
        {
            databaseLogics.databaseLogicMessages.storeImage(t0028);
            std::map<std::string, std::string> data;
            data["id"] = t0028.id.asString();
            answerOk("image stored", true, data);
            return;
        }
        if (isGet())
        {
            // FIXME FIX ME AND IMPLEMENT ME
        }
        if (isDelete())
        {
            // FIXME FIX ME AND IMPLEMENT ME
        }
        return;
    }

    MACRO_GetMandatoryUuid(id);
    MACRO_GetMandatoryUuid(to_id);
    MACRO_GetMandatoryString(content_base64);

    databaseLogics.databaseLogicMessages.storeMessage(id,
                                                      context.userId,
                                                      to_id,
                                                      content_base64);
    std::set<std::string> deviceToken;
    if (deviceTokenCache.get(context, to_id, deviceToken))
    {
        jw78::FirebaseWrapper fw;
        for (const auto &dt: deviceToken)
        {
            databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::info, std::string("got devicetoken: ") + dt);
            rapidjson::Document payloadJson(rapidjson::kObjectType);
            payloadJson.SetObject();
            payloadJson.AddMember("to", dt, payloadJson.GetAllocator());
            rapidjson::Value notification;
            notification.SetObject();
            notification.AddMember("title", "test", payloadJson.GetAllocator());
            notification.AddMember("body", "body", payloadJson.GetAllocator());
            payloadJson.AddMember("notification", notification, payloadJson.GetAllocator());
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            payloadJson.Accept(writer);
            std::string result;
            if (fw.legacySend(firebaseApiKey,
                              buffer.GetString(),
                              result))
            {
                if (false /* deviceToken invalid*/)
                {
                    deviceTokenCache.remove(context, to_id, dt);
                }
            }
        }
    }
    else
    {
        databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::info, std::string("got no devicetoken"));
    }
    answerOk("message stored", true);
}
