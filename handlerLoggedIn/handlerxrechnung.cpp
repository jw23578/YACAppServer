#include "handlerxrechnung.h"
#include "base64.h"
#include "rapidjson/error/en.h"
#include "xrechnunggenerator.h"

HandlerXRechnung::HandlerXRechnung(PistacheServerInterface &serverInterface,
                                   LoggedInAppUsersContainer &loggedInAppUsersContainer,
                                   PGORMPersistence &opi):HandlerLoggedInInterface(serverInterface, opi, loggedInAppUsersContainer)
{
    addMethod(serverInterface, "generateXRechnung", MethodInfo::TypeGet);
}

void HandlerXRechnung::method(CurrentContext &context)
{
    MACRO_GetMandatoryString(invoiceDataJSONBase64);
    std::string invoiceDataJSON(base64_decode(invoiceDataJSONBase64));
    if (!invoiceDataJSON.size())
    {
        answerOk("invoiceDataJSON is empty", false);
        return;
    }
    rapidjson::Document invoiceData;
    invoiceData.Parse(invoiceDataJSON);
    if (invoiceData.HasParseError())
    {
        std::map<std::string, std::string> answerData;
        answerData["error"] = rapidjson::GetParseError_En(invoiceData.GetParseError());
        answerBad("invoiceData is not valid json", answerData);
        return;
    }
    Party seller;
    Party buyer;
    std::vector<LineItem> itemsVector;
    BankAccount paymentAccount;

    ExtRapidJSON id(invoiceData);
    if (id.hasObject("seller"))
    {
        ExtRapidJSON s(id.getObject("seller"));
        seller.name = s.getString("name");
        seller.vatId = s.getString("vatId");
        seller.id.push_back(s.getString("id"));
        if (s.hasObject("address"))
        {
            ExtRapidJSON a(s.getObject("address"));
            seller.address.lineOne = a.getString("lineone");
            seller.address.postcode = a.getString("postcode");
            seller.address.city = a.getString("city");
            seller.address.countryCode = a.getString("countryCode");
        }
        if (s.hasObject("contact"))
        {
            ExtRapidJSON c(s.getObject("contact"));
            seller.contact.name = c.getString("name");
            seller.contact.telephone = c.getString("telephone");
            seller.contact.email = c.getString("email");
        }
    }
    if (id.hasObject("buyer"))
    {
        ExtRapidJSON b(id.getObject("buyer"));
        buyer.name = b.getString("name");
        buyer.id.push_back(b.getString("id"));
        buyer.vatId = b.getString("vatId");
        if (b.hasObject("address"))
        {
            ExtRapidJSON a(b.getObject("address"));
            buyer.address.lineOne = a.getString("lineone");
            buyer.address.postcode = a.getString("postcode");
            buyer.address.city = a.getString("city");
            buyer.address.countryCode = a.getString("countryCode");
        }
    }
    if (id.hasArray("items"))
    {
        const rapidjson::Value &items(id.getObject("items"));
        for (size_t i(0); i < items.Size(); ++i)
        {
            ExtRapidJSON item(items[i]);
            LineItem item1;
            item1.id = item.getString("id");
            item1.name = item.getString("name");
            item1.description = item.getString("description");
            item1.quantity = item.getDouble("quantity");
            item1.unitCode = item.getString("unitCode");
            item1.netPrice = item.getDouble("netPrice");
            item1.lineTotal = item1.quantity * item1.netPrice;
            if (item.hasObject("tax"))
            {
                ExtRapidJSON tax(item.getObject("tax"));
                item1.tax.categoryCode = tax.getString("categoryCode"); // "S";  // S = Standard rate
                item1.tax.percent = tax.getDouble("percent");
            }
            itemsVector.push_back(item1);
        }
    }
    if (id.hasObject("paymentAccount"))
    {
        ExtRapidJSON pa(id.getObject("paymentAccount"));
        paymentAccount.iban = pa.getString("iban");
        paymentAccount.accountName = pa.getString("accountName");
        paymentAccount.bic = pa.getString("bic");
    }
    std::string errorMessage;

    // XRechnung generieren
    XRechnungGenerator generator;
    std::string xmlContent = generator.generateXRechnung(
        id.getString("invoiceNumber"),
        id.getString("invoiceDate"),
        id.getString("buyerReference"),
        seller,
        buyer,
        itemsVector,
        paymentAccount,
        id.getString("paymentTerms"),
        id.getString("dueDate"),
        errorMessage);

    // Prüfen ob Generierung erfolgreich war
    if (xmlContent.empty())
    {
        answerOk(errorMessage, false);
        return;
    }
    rapidjson::Document document;
    document.SetObject();
    ExtRapidJSONWriter answer(document, document.GetAllocator());
    answer.addMember("xmlBase64", base64_encode(xmlContent));
    answer.addMember("message", "XRechnung created");
    answerOk(true, document);
}
