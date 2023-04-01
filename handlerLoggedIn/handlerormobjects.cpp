#include "handlerormobjects.h"

HandlerORMObjects::HandlerORMObjects(ORMFactory &factory,
                                     PGConnectionPool &pool,
                                     RightsLogic &rightsLogic,
                                     PistacheServerInterface &serverInterface,
                                     LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             "",
                             TypeGet,
                             loggedInAppUsersContainer),
    factory(factory),    
    o2postgres(pool),
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

    }
    if (isPost())
    {
        for (const auto &on: factory.getORMNames())
        {
            if (isMethod(on))
            {
                std::unique_ptr<YACBaseObject> object(o2json.fromJson(getPostedData(), factory));
                if (answerMissingRight(rightsLogic.appUserMissesRight(loggedInUserId, object->changeRight)))
                {
                    return;
                }
                o2postgres.insertOrUpdate(*object);
                answerOk("object stored", true);
                return;
            }
        }
    }
}
