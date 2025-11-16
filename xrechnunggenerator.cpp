#include "xrechnunggenerator.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <map>

XRechnungGenerator::XRechnungGenerator() {}

XRechnungGenerator::~XRechnungGenerator() {}

std::string XRechnungGenerator::generateXRechnung(
    const std::string& invoiceNumber,
    const std::string& invoiceDate,
    const std::string& buyerReference,
    const Party& seller,
    const Party& buyer,
    const std::vector<LineItem>& items,
    const BankAccount& paymentAccount,
    const std::string& paymentTerms,
    const std::string& dueDate,
    std::string &errorMessage)
{
    // Validierung der Eingabedaten
    if (!validateInputData(invoiceNumber, invoiceDate, buyerReference, seller, buyer,
                           items, paymentAccount, errorMessage))
    {
        return "";
    }

    // XML-Dokument erstellen
    doc.reset();
    createDocumentStructure();

    pugi::xml_node root = doc.child("rsm:CrossIndustryInvoice");

    // Hauptbestandteile hinzufügen
    addExchangedDocumentContext(root);
    addExchangedDocument(root, invoiceNumber, invoiceDate);
    addSupplyChainTradeTransaction(root, buyerReference, seller, buyer, items,
                                   paymentAccount, paymentTerms, dueDate);

    // XML in String konvertieren
    std::ostringstream oss;
    doc.save(oss, "  ", pugi::format_default, pugi::encoding_utf8);

    return oss.str();
}

void XRechnungGenerator::createDocumentStructure() {
    // XML-Deklaration
    pugi::xml_node declaration = doc.prepend_child(pugi::node_declaration);
    declaration.append_attribute("version") = "1.0";
    declaration.append_attribute("encoding") = "UTF-8";

    // Root-Element mit allen Namespaces
    pugi::xml_node root = doc.append_child("rsm:CrossIndustryInvoice");
    root.append_attribute("xmlns:rsm") = "urn:un:unece:uncefact:data:standard:CrossIndustryInvoice:100";
    root.append_attribute("xmlns:qdt") = "urn:un:unece:uncefact:data:standard:QualifiedDataType:100";
    root.append_attribute("xmlns:ram") = "urn:un:unece:uncefact:data:standard:ReusableAggregateBusinessInformationEntity:100";
    root.append_attribute("xmlns:xs") = "http://www.w3.org/2001/XMLSchema";
    root.append_attribute("xmlns:udt") = "urn:un:unece:uncefact:data:standard:UnqualifiedDataType:100";
}

void XRechnungGenerator::addExchangedDocumentContext(pugi::xml_node& root) {
    pugi::xml_node context = root.append_child("rsm:ExchangedDocumentContext");

    // Geschäftsprozess
    pugi::xml_node businessProcess = context.append_child("ram:BusinessProcessSpecifiedDocumentContextParameter");
    businessProcess.append_child("ram:ID").text() = "A1";

    // Leitweg-ID für XRechnung
    pugi::xml_node guideline = context.append_child("ram:GuidelineSpecifiedDocumentContextParameter");
    guideline.append_child("ram:ID").text() = "urn:cen.eu:en16931:2017#compliant#urn:xeinkauf.de:kosit:xrechnung_3.0";
}

void XRechnungGenerator::addExchangedDocument(pugi::xml_node& root,
                                              const std::string& invoiceNumber,
                                              const std::string& invoiceDate) {
    pugi::xml_node document = root.append_child("rsm:ExchangedDocument");

    // Rechnungsnummer
    document.append_child("ram:ID").text() = invoiceNumber.c_str();

    // Dokumententyp (380 = Rechnung)
    document.append_child("ram:TypeCode").text() = "380";

    // Rechnungsdatum
    pugi::xml_node issueDateTime = document.append_child("ram:IssueDateTime");
    pugi::xml_node dateTimeString = issueDateTime.append_child("udt:DateTimeString");
    dateTimeString.append_attribute("format") = "102";
    dateTimeString.text() = invoiceDate.c_str();
}

void XRechnungGenerator::addSupplyChainTradeTransaction(
    pugi::xml_node& root,
    const std::string& buyerReference,
    const Party& seller,
    const Party& buyer,
    const std::vector<LineItem>& items,
    const BankAccount& paymentAccount,
    const std::string& paymentTerms,
    const std::string& dueDate
    ) {
    pugi::xml_node transaction = root.append_child("rsm:SupplyChainTradeTransaction");

    // Positionsdaten (müssen vor ApplicableHeaderTradeAgreement kommen)
    addLineItems(transaction, items);

    // ApplicableHeaderTradeAgreement
    pugi::xml_node agreement = transaction.append_child("ram:ApplicableHeaderTradeAgreement");

    // BR-DE-15: Buyer Reference (Pflichtfeld)
    agreement.append_child("ram:BuyerReference").text() = buyerReference.c_str();

    // Verkäufer
    addParty(agreement, "ram:SellerTradeParty", seller);

    // Käufer
    addParty(agreement, "ram:BuyerTradeParty", buyer);

    // ApplicableHeaderTradeDelivery
    pugi::xml_node delivery = transaction.append_child("ram:ApplicableHeaderTradeDelivery");
    pugi::xml_node actualDelivery = delivery.append_child("ram:ActualDeliverySupplyChainEvent");
    pugi::xml_node occurrenceDateTime = actualDelivery.append_child("ram:OccurrenceDateTime");
    pugi::xml_node deliveryDateString = occurrenceDateTime.append_child("udt:DateTimeString");
    deliveryDateString.append_attribute("format") = "102";
    deliveryDateString.text() = "20250101";

    // ApplicableHeaderTradeSettlement
    addSettlement(transaction, items, paymentAccount, paymentTerms, dueDate);
}

void XRechnungGenerator::addParty(pugi::xml_node& parent, const std::string& nodeName,
                                  const Party& party) {
    pugi::xml_node partyNode = parent.append_child(nodeName.c_str());

    // IDs
    for (const auto& id : party.id) {
        partyNode.append_child("ram:ID").text() = id.c_str();
    }

    // Name
    partyNode.append_child("ram:Name").text() = party.name.c_str();

    // BR-DE-2: Seller Contact ist Pflicht beim Verkäufer
    if (nodeName == "ram:SellerTradeParty")
    {
        addContact(partyNode, party.contact);
    }

    // Adresse
    addAddress(partyNode, party.address);

    // <ram:URIUniversalCommunication>
    //     <ram:URIID schemeID="9930">DE123456789</ram:URIID>
    //     </ram:URIUniversalCommunication>

    auto URIUniversalCommunication = partyNode.append_child("ram:URIUniversalCommunication");
    auto URIID = URIUniversalCommunication.append_child("ram:URIID");
    URIID.append_attribute("schemeID") = "9930";
    URIID.text() = party.vatId.c_str();

    // USt-IdNr.
    pugi::xml_node taxReg = partyNode.append_child("ram:SpecifiedTaxRegistration");
    pugi::xml_node taxId = taxReg.append_child("ram:ID");
    taxId.append_attribute("schemeID") = "VA";
    taxId.text() = party.vatId.c_str();
}

void XRechnungGenerator::addAddress(pugi::xml_node& parent, const Address& address) {
    pugi::xml_node postalAddress = parent.append_child("ram:PostalTradeAddress");

    if (!address.postcode.empty()) {
        postalAddress.append_child("ram:PostcodeCode").text() = address.postcode.c_str();
    }

    if (!address.lineOne.empty()) {
        postalAddress.append_child("ram:LineOne").text() = address.lineOne.c_str();
    }

    if (!address.lineTwo.empty()) {
        postalAddress.append_child("ram:LineTwo").text() = address.lineTwo.c_str();
    }

    if (!address.city.empty()) {
        postalAddress.append_child("ram:CityName").text() = address.city.c_str();
    }

    postalAddress.append_child("ram:CountryID").text() = address.countryCode.c_str();
}

void XRechnungGenerator::addContact(pugi::xml_node& parent, const Contact& contact) {
    pugi::xml_node contactNode = parent.append_child("ram:DefinedTradeContact");

    if (!contact.name.empty()) {
        contactNode.append_child("ram:PersonName").text() = contact.name.c_str();
    }

    if (!contact.telephone.empty()) {
        pugi::xml_node telephone = contactNode.append_child("ram:TelephoneUniversalCommunication");
        telephone.append_child("ram:CompleteNumber").text() = contact.telephone.c_str();
    }

    if (!contact.email.empty()) {
        pugi::xml_node email = contactNode.append_child("ram:EmailURIUniversalCommunication");
        email.append_child("ram:URIID").text() = contact.email.c_str();
    }
}

void XRechnungGenerator::addLineItems(pugi::xml_node& parent,
                                      const std::vector<LineItem>& items) {
    for (const auto& item : items) {
        pugi::xml_node lineItem = parent.append_child("ram:IncludedSupplyChainTradeLineItem");

        // Positionsnummer
        pugi::xml_node associatedDoc = lineItem.append_child("ram:AssociatedDocumentLineDocument");
        associatedDoc.append_child("ram:LineID").text() = item.id.c_str();

        // Produktinformationen
        pugi::xml_node product = lineItem.append_child("ram:SpecifiedTradeProduct");
        product.append_child("ram:Name").text() = item.name.c_str();

        if (!item.description.empty()) {
            product.append_child("ram:Description").text() = item.description.c_str();
        }

        // Positionsvereinbarung
        pugi::xml_node lineAgreement = lineItem.append_child("ram:SpecifiedLineTradeAgreement");
        pugi::xml_node netPrice = lineAgreement.append_child("ram:NetPriceProductTradePrice");
        netPrice.append_child("ram:ChargeAmount").text() = formatAmount(item.netPrice).c_str();

        // Lieferung
        pugi::xml_node lineDelivery = lineItem.append_child("ram:SpecifiedLineTradeDelivery");
        pugi::xml_node billedQty = lineDelivery.append_child("ram:BilledQuantity");
        billedQty.append_attribute("unitCode") = item.unitCode.c_str();
        billedQty.text() = formatAmount(item.quantity).c_str();

        // Abrechnung
        pugi::xml_node lineSettlement = lineItem.append_child("ram:SpecifiedLineTradeSettlement");

        // Steuern
        pugi::xml_node lineTax = lineSettlement.append_child("ram:ApplicableTradeTax");
        lineTax.append_child("ram:TypeCode").text() = "VAT";
        lineTax.append_child("ram:CategoryCode").text() = item.tax.categoryCode.c_str();
        lineTax.append_child("ram:RateApplicablePercent").text() = formatAmount(item.tax.percent).c_str();

        // Positionssumme
        pugi::xml_node monetary = lineSettlement.append_child("ram:SpecifiedTradeSettlementLineMonetarySummation");
        monetary.append_child("ram:LineTotalAmount").text() = formatAmount(item.lineTotal).c_str();
    }
}

void XRechnungGenerator::addSettlement(pugi::xml_node& parent,
                                       const std::vector<LineItem>& items,
                                       const BankAccount& paymentAccount,
                                       const std::string& paymentTerms,
                                       const std::string& dueDate) {
    pugi::xml_node settlement = parent.append_child("ram:ApplicableHeaderTradeSettlement");

    // 1. Währung - muss als erstes kommen
    settlement.append_child("ram:InvoiceCurrencyCode").text() = "EUR";

    // 2. BR-DE-1: Payment Means - kommt direkt nach InvoiceCurrencyCode, VOR allen anderen Elementen!
    addPaymentMeans(settlement, paymentAccount);

    // 3. Steuerzusammenfassung - kommt nach PaymentMeans
    addTaxSummary(settlement, items);

    // 4. Zahlungsbedingungen (optional) - kommt nach ApplicableTradeTax
    if (!dueDate.empty() || !paymentTerms.empty()) {
        pugi::xml_node paymentTermsNode = settlement.append_child("ram:SpecifiedTradePaymentTerms");

        if (!paymentTerms.empty()) {
            paymentTermsNode.append_child("ram:Description").text() = paymentTerms.c_str();
        }

        if (!dueDate.empty()) {
            pugi::xml_node dueDateNode = paymentTermsNode.append_child("ram:DueDateDateTime");
            pugi::xml_node dueDateString = dueDateNode.append_child("udt:DateTimeString");
            dueDateString.append_attribute("format") = "102";
            dueDateString.text() = dueDate.c_str();
        }
    }

    // 5. Monetäre Zusammenfassung - kommt zum Schluss
    addMonetarySummation(settlement, items);
}

void XRechnungGenerator::addPaymentMeans(pugi::xml_node& parent, const BankAccount& account) {
    pugi::xml_node paymentMeans = parent.append_child("ram:SpecifiedTradeSettlementPaymentMeans");

    // Zahlungsart (58 = SEPA Überweisung)
    paymentMeans.append_child("ram:TypeCode").text() = "58";

    // Bankverbindung
    pugi::xml_node payeeAccount = paymentMeans.append_child("ram:PayeePartyCreditorFinancialAccount");
    payeeAccount.append_child("ram:IBANID").text() = account.iban.c_str();

    if (!account.accountName.empty()) {
        payeeAccount.append_child("ram:AccountName").text() = account.accountName.c_str();
    }

    if (!account.bic.empty()) {
        pugi::xml_node institution = paymentMeans.append_child("ram:PayeeSpecifiedCreditorFinancialInstitution");
        institution.append_child("ram:BICID").text() = account.bic.c_str();
    }
}

void XRechnungGenerator::addTaxSummary(pugi::xml_node& parent,
                                       const std::vector<LineItem>& items) {
    // Steuersätze gruppieren mit präziser Berechnung
    std::map<std::string, std::pair<double, double>> taxGroups; // key: categoryCode, value: (basis, tax)

    for (const auto& item : items) {
        std::string key = item.tax.categoryCode;
        double itemBasis = item.lineTotal;
        // Steuer pro Position berechnen und auf 2 Dezimalstellen runden
        double itemTax = std::round(itemBasis * item.tax.percent / 100.0 * 100.0) / 100.0;

        taxGroups[key].first += itemBasis;
        taxGroups[key].second += itemTax;
    }

    // Für jeden Steuersatz einen ApplicableTradeTax-Eintrag erstellen
    for (const auto& [categoryCode, amounts] : taxGroups) {
        pugi::xml_node tax = parent.append_child("ram:ApplicableTradeTax");
        tax.append_child("ram:CalculatedAmount").text() = formatAmount(amounts.second).c_str();
        tax.append_child("ram:TypeCode").text() = "VAT";
        tax.append_child("ram:BasisAmount").text() = formatAmount(amounts.first).c_str();
        tax.append_child("ram:CategoryCode").text() = categoryCode.c_str();

        // Steuersatz aus dem ersten Item mit diesem Code nehmen
        for (const auto& item : items) {
            if (item.tax.categoryCode == categoryCode) {
                tax.append_child("ram:RateApplicablePercent").text() = formatAmount(item.tax.percent).c_str();
                break;
            }
        }
    }
}

void XRechnungGenerator::addMonetarySummation(pugi::xml_node& parent,
                                              const std::vector<LineItem>& items) {
    pugi::xml_node monetary = parent.append_child("ram:SpecifiedTradeSettlementHeaderMonetarySummation");

    double totalNet = calculateTotalNet(items);
    double totalTax = calculateTotalTax(items);
    double totalGross = calculateTotalGross(items);

    monetary.append_child("ram:LineTotalAmount").text() = formatAmount(totalNet).c_str();
    monetary.append_child("ram:TaxBasisTotalAmount").text() = formatAmount(totalNet).c_str();
    auto TaxTotalAmount = monetary.append_child("ram:TaxTotalAmount");
    TaxTotalAmount.text() = formatAmount(totalTax).c_str();
    TaxTotalAmount.append_attribute("currencyID").set_value("EUR");
    monetary.append_child("ram:GrandTotalAmount").text() = formatAmount(totalGross).c_str();
    monetary.append_child("ram:DuePayableAmount").text() = formatAmount(totalGross).c_str();
}

double XRechnungGenerator::calculateTotalNet(const std::vector<LineItem>& items) {
    double total = 0.0;
    for (const auto& item : items) {
        total += item.lineTotal;
    }
    return total;
}

#include <iostream>
double XRechnungGenerator::calculateTotalTax(const std::vector<LineItem>& items) {
    double total = 0.0;
    for (const auto& item : items) {
        // Steuer pro Position berechnen und auf 2 Dezimalstellen runden (wie in addTaxSummary)
        double itemTax = std::round(item.lineTotal * item.tax.percent) / 100.0;
        total += itemTax;
        std::cout << item.lineTotal << " " << itemTax << std::endl;
    }
    return total;
}

double XRechnungGenerator::calculateTotalGross(const std::vector<LineItem>& items) {
    return calculateTotalNet(items) + calculateTotalTax(items);
}

std::string XRechnungGenerator::formatAmount(double amount, int decimals) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimals) << amount;
    return oss.str();
}

bool XRechnungGenerator::validateInputData(
    const std::string& invoiceNumber,
    const std::string& invoiceDate,
    const std::string& buyerReference,
    const Party& seller,
    const Party& buyer,
    const std::vector<LineItem>& items,
    const BankAccount& paymentAccount,
    std::string& errorMsg
    ) {
    // Rechnungsnummer prüfen
    if (invoiceNumber.empty()) {
        errorMsg = "Fehler: Rechnungsnummer darf nicht leer sein.";
        return false;
    }

    // Rechnungsdatum prüfen (Format YYYYMMDD)
    if (invoiceDate.length() != 8) {
        errorMsg = "Fehler: Rechnungsdatum muss im Format YYYYMMDD angegeben werden (z.B. 20250115).";
        return false;
    }

    // BR-DE-15: Buyer Reference prüfen
    if (buyerReference.empty()) {
        errorMsg = "Fehler [BR-DE-15]: Buyer Reference (BT-10) ist ein Pflichtfeld und darf nicht leer sein.";
        return false;
    }

    // Verkäufer validieren
    if (!validateParty(seller, "Verkäufer", errorMsg)) {
        return false;
    }

    if (seller.contact.name.empty())
    {
        errorMsg = "Fehler [BR-DE-2]: Name muss beim Verkäufer angegeben werden.";
        return false;
    }

    if (seller.contact.telephone.empty())
    {
        errorMsg = "Fehler [BR-DE-2]: Telefon muss beim Verkäufer angegeben werden.";
        return false;
    }

    if (seller.contact.email.empty())
    {
        errorMsg = "Fehler [BR-DE-2]: E-Mail muss beim Verkäufer angegeben werden.";
        return false;
    }

    // Käufer validieren
    if (!validateParty(buyer, "Käufer", errorMsg)) {
        return false;
    }

    // BR-DE-1: Zahlungsinformationen prüfen
    if (!validateBankAccount(paymentAccount, errorMsg)) {
        return false;
    }

    // Positionen prüfen
    if (!validateLineItems(items, errorMsg)) {
        return false;
    }

    return true;
}

bool XRechnungGenerator::validateParty(const Party& party, const std::string& partyType,
                                       std::string& errorMsg) {
    if (party.name.empty()) {
        errorMsg = "Fehler: " + partyType + "-Name darf nicht leer sein.";
        return false;
    }

    if (!validateAddress(party.address, partyType, errorMsg)) {
        return false;
    }

    return true;
}

bool XRechnungGenerator::validateAddress(const Address& addr, const std::string& partyType,
                                         std::string& errorMsg) {
    if (addr.countryCode.empty()) {
        errorMsg = "Fehler: Ländercode für " + partyType + "-Adresse darf nicht leer sein.";
        return false;
    }

    if (addr.countryCode.length() != 2) {
        errorMsg = "Fehler: Ländercode muss aus 2 Buchstaben bestehen (z.B. DE, AT, CH).";
        return false;
    }

    return true;
}

bool XRechnungGenerator::validateBankAccount(const BankAccount& account, std::string& errorMsg) {
    if (account.iban.empty()) {
        errorMsg = "Fehler [BR-DE-1]: IBAN für Zahlungsinformationen (PAYMENT INSTRUCTIONS) darf nicht leer sein.";
        return false;
    }

    // Einfache IBAN-Validierung (mindestens 15 Zeichen)
    if (account.iban.length() < 15) {
        errorMsg = "Fehler: IBAN scheint ungültig zu sein (zu kurz).";
        return false;
    }

    return true;
}

bool XRechnungGenerator::validateLineItems(const std::vector<LineItem>& items,
                                           std::string& errorMsg) {
    if (items.empty()) {
        errorMsg = "Fehler: Rechnung muss mindestens eine Position enthalten.";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        const auto& item = items[i];

        if (item.name.empty()) {
            errorMsg = "Fehler: Position " + std::to_string(i + 1) + " - Produktname darf nicht leer sein.";
            return false;
        }

        if (item.quantity <= 0) {
            errorMsg = "Fehler: Position " + std::to_string(i + 1) + " - Menge muss größer als 0 sein.";
            return false;
        }

        if (item.netPrice < 0) {
            errorMsg = "Fehler: Position " + std::to_string(i + 1) + " - Nettopreis darf nicht negativ sein.";
            return false;
        }

        if (item.unitCode.empty()) {
            errorMsg = "Fehler: Position " + std::to_string(i + 1) + " - Einheitencode darf nicht leer sein (z.B. C62 für Stück).";
            return false;
        }

        // Prüfe ob lineTotal mit quantity * netPrice übereinstimmt (mit Toleranz)
        double expectedTotal = item.quantity * item.netPrice;
        if (std::abs(expectedTotal - item.lineTotal) > 0.01) {
            errorMsg = "Warnung: Position " + std::to_string(i + 1) + " - Positionssumme (" +
                       formatAmount(item.lineTotal) + ") stimmt nicht mit Menge x Preis (" +
                       formatAmount(expectedTotal) + ") überein.";
        }
    }

    return true;
}
