#include "handlerormobjects.h"
#include "orm_implementions/t0027_app_images.h"
#include "orm_implementions/t0028_message_images.h"
#include "orm_implementions/t0030_documents.h"
#include "orm_implementions/t0023_right2rightgroup.h"

void HandlerORMObjects::storeObject(YACBaseObject &object)
{
    orm2postgres.insertOrUpdate(object);
    std::map<std::string, std::string> data;
    data[tableFields.id] = object.getPropertyToString(tableFields.id);
    answerOk("object stored", true, data);
}

HandlerORMObjects::HandlerORMObjects(DatabaseLogics &databaseLogics,
                                     YACORMFactory &factory,
                                     PGConnectionPool &pool,
                                     RightsLogic &rightsLogic,
                                     PistacheServerInterface &serverInterface,
                                     LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
                             "",
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics),
    factory(factory),
    orm2postgres(pool),
    rightsLogic(rightsLogic)
{
    std::set<std::string> withOwnHandler;
    withOwnHandler.insert(t0027_app_images().getORMName());
    withOwnHandler.insert(t0028_message_images().getORMName());
    withOwnHandler.insert(t0030_documents().getORMName());
    for (const auto &on: factory.getORMNames())
    {
        if (withOwnHandler.find(on) == withOwnHandler.end())
        {
            addMethod(serverInterface,
                      on,
                      TypeGet);
            addMethod(serverInterface,
                      on,
                      TypePost);
            addMethod(serverInterface,
                      on,
                      TypeDelete);
        }
    }

}

void HandlerORMObjects::method(CurrentContext &context)
{
    if (isGet())
    {
        if (isMethod(t0021.getORMName()))
        {
            MACRO_GetUuid(id);
            if (id == ExtUuid::NullUuid)
            {
                rapidjson::Document document;
                document.SetObject();
                rapidjson::Value rightgroups;

                SqlString sql;
                sql.select(t0021.getORMName());
                sql.addCompare("where", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
                sql.addCompare("and", tableFields.app_id, "=", context.appId);
                orm2postgres.toJsonArray(sql, t0021, rightgroups, document.GetAllocator());
                document.AddMember("t0021_right_group", rightgroups, document.GetAllocator());

                context.userId = context.userId;
                databaseLogics.rightsLogic.addUserRights(context, document, document.GetAllocator());
                answerOk(true, document);
                return;
            }
        }

        if (isMethod(t0023_right2rightgroup().getORMName()))
        {
            MACRO_GetMandatoryUuid(right_group_id);
            rapidjson::Document document;
            document.SetObject();
            rapidjson::Value array;
            orm2postgres.selectAll(t0023_right2rightgroup(),
                                   "right_group_id",
                                   right_group_id,
                                   array,
                                   document.GetAllocator());
            document.AddMember("t0023_right2rightgroup", array, document.GetAllocator());
            answerOk(true, document);
            return;
        }
    }
    if (isPost())
    {
        if (isMethod(t0022.getORMName()))
        {
            std::unique_ptr<t0022_right_group2user> object(static_cast<t0022_right_group2user*>(orm2json.fromJson(getPostedData(), factory)));

            t0022_right_group2user rightGroup2User;
            if (rightGroup2User.userInRightGroup(context, object->right_group_id.get(), object->user_id.get()))
            {
                return;
            }
            storeObject(*object);
            return;
        }
        for (const auto &on: factory.getORMNames())
        {
            if (isMethod(on))
            {
                std::unique_ptr<YACBaseObject> baseObject(static_cast<YACBaseObject*>(orm2json.fromJson(getPostedData(), factory)));
                if (answerMissingRight(rightsLogic.appUserMissesRight(context, context.userId, baseObject->changeRight)))
                {
                    return;
                }
                if (baseObject->propertyExists(tableFields.app_id) && baseObject->propertyIsNull(tableFields.app_id))
                {
                    baseObject->setPropertyFromString(tableFields.app_id, context.appId.str());
                }
                if (baseObject->propertyExists(tableFields.creater_id) && baseObject->propertyIsNull(tableFields.creater_id))
                {
                    baseObject->setPropertyFromString(tableFields.creater_id, context.userId.str());
                }
                storeObject(*baseObject);
                return;
            }
        }
    }
}
