#include "handleribancheck.h"
#include "ibancheck.h"

HandlerIBANCheck::HandlerIBANCheck(ORMPersistenceInterface &opi,
                                   PistacheServerInterface &serverInterface,
                                   LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             opi,
                             loggedInAppUsersContainer)
{
    addMethod(serverInterface, methodNames.checkIBAN, TypeGet);
}

void HandlerIBANCheck::method(CurrentContext &context)
{
    MACRO_GetMandatoryString(iban);
    ExtString::upperSelf(iban);
    bool checksumOk(false);
    bool nationIsKnown(false);
    bool ibanOk(IBANCheck::check(iban,
                                 checksumOk,
                                 nationIsKnown));
    rapidjson::Document document;
    document.SetObject();
    ExtRapidJSONWriter answer(document, document.GetAllocator());
    answer.addMember("nationIsKnown", nationIsKnown);
    answer.addMember("checksumOk", checksumOk);
    answerOk(ibanOk, document);
}
