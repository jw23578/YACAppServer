#ifndef XRECHNUNG_GENERATOR_H
#define XRECHNUNG_GENERATOR_H

#include <string>
#include <vector>
#include <optional>
#include "pugixml.hpp"

// Strukturen für die verschiedenen Datenkomponenten
struct Address {
    std::string lineOne;
    std::string lineTwo;
    std::string lineThree;
    std::string postcode;
    std::string city;
    std::string countryCode;
};

struct Contact {
    std::string name;
    std::string telephone;
    std::string email;
};

struct Party {
    std::string name;
    std::string tradingName;
    std::vector<std::string> id;
    Address address;
    Contact contact;
    std::string vatId;
};

struct BankAccount {
    std::string iban;
    std::string accountName;
    std::string bic;
};

struct TaxInfo {
    std::string categoryCode;  // z.B. "S" für Standard rate
    double percent;
};

struct LineItem {
    std::string id;
    std::string name;
    std::string description;
    double quantity;
    std::string unitCode;  // z.B. "C62" für Stück
    double netPrice;
    double lineTotal;
    TaxInfo tax;
};

class XRechnungGenerator {
public:
    XRechnungGenerator();
    ~XRechnungGenerator();

    // Hauptmethode zur Erstellung der XRechnung
    std::string generateXRechnung(
        const std::string& invoiceNumber,
        const std::string& invoiceDate,
        const std::string& buyerReference,  // BR-DE-15: Pflichtfeld
        const Party& seller,
        const Party& buyer,
        const std::vector<LineItem>& items,
        const BankAccount& paymentAccount,
        const std::string& paymentTerms,
        const std::string& dueDate,
        std::string &errorMessage);

private:
    pugi::xml_document doc;

    // Validierungsmethoden
    bool validateInputData(
        const std::string& invoiceNumber,
        const std::string& invoiceDate,
        const std::string& buyerReference,
        const Party& seller,
        const Party& buyer,
        const std::vector<LineItem>& items,
        const BankAccount& paymentAccount,
        std::string& errorMsg
        );

    bool validateParty(const Party& party, const std::string& partyType, std::string& errorMsg);
    bool validateAddress(const Address& addr, const std::string& partyType, std::string& errorMsg);
    bool validateBankAccount(const BankAccount& account, std::string& errorMsg);
    bool validateLineItems(const std::vector<LineItem>& items, std::string& errorMsg);

    // XML-Erstellungsmethoden
    void createDocumentStructure();
    void addExchangedDocumentContext(pugi::xml_node& root);
    void addExchangedDocument(pugi::xml_node& root, const std::string& invoiceNumber,
                              const std::string& invoiceDate);
    void addSupplyChainTradeTransaction(pugi::xml_node& root, const std::string& buyerReference,
                                        const Party& seller, const Party& buyer,
                                        const std::vector<LineItem>& items,
                                        const BankAccount& paymentAccount,
                                        const std::string& paymentTerms,
                                        const std::string& dueDate);
    void addParty(pugi::xml_node& parent, const std::string& nodeName, const Party& party);
    void addAddress(pugi::xml_node& parent, const Address& address);
    void addContact(pugi::xml_node& parent, const Contact& contact);
    void addLineItems(pugi::xml_node& parent, const std::vector<LineItem>& items);
    void addSettlement(pugi::xml_node& parent, const std::vector<LineItem>& items,
                       const BankAccount& paymentAccount, const std::string& paymentTerms,
                       const std::string& dueDate);
    void addPaymentMeans(pugi::xml_node& parent, const BankAccount& account);
    void addTaxSummary(pugi::xml_node& parent, const std::vector<LineItem>& items);
    void addMonetarySummation(pugi::xml_node& parent, const std::vector<LineItem>& items);

    // Hilfsmethoden
    double calculateTotalNet(const std::vector<LineItem>& items);
    double calculateTotalTax(const std::vector<LineItem>& items);
    double calculateTotalGross(const std::vector<LineItem>& items);
    std::string formatAmount(double amount, int decimals = 2);
};

#endif // XRECHNUNG_GENERATOR_H
