#include "handlerormobjects.h"
#include "orm_implementions/t0023_right2rightgroup.h"
#include "orm_implementions/t0022_right_group2appuser.h"

HandlerORMObjects::HandlerORMObjects(YACORMFactory &factory,
                                     PGConnectionPool &pool,
                                     RightsLogic &rightsLogic,
                                     PistacheServerInterface &serverInterface,
                                     LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "",
                             TypeGet,
                             loggedInAppUsersContainer),
    factory(factory),    
    orm2postgres(pool),
    rightsLogic(rightsLogic)
{
    for (const auto &on: factory.getORMNames())
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

void HandlerORMObjects::method()
{
    if (isGet())
    {
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
        for (const auto &on: factory.getORMNames())
        {
            if (isMethod(on))
            {
                YACBaseObject *baseObject(static_cast<YACBaseObject*>(orm2json.fromJson(getPostedData(), factory)));
                std::unique_ptr<ORMObjectInterface> object(baseObject);
                if (answerMissingRight(rightsLogic.appUserMissesRight(loggedInUserId, baseObject->changeRight)))
                {
                    return;
                }
                orm2postgres.insertOrUpdate(*object);
                answerOk("object stored", true);
                return;
            }
        }
    }
}
