#include "BankAccount.h"
#include "AccountRegistry.h"
#include "GwiezdnaKomisjaBankowa.h"
#include <sstream>
#include <exception>
#include <iomanip>
#include <cmath>

IdType Account::id() const {
    return id_;
}

money_t Account::balanceNumber() const {
    return balance_;
}

std::string Account::balance() const {
    std::stringstream ss;
    ss << MoneyFormat(balance_, currency());
    return ss.str();
}

std::string Account::history() const {
    return log_;
}

void Account::onEveryMonth() {
    if (info().interestRate != 0 && balance_ > 0) {
       capitalizeInterest();
    }
    if (info().monthlyCharge != 0) {
        charge(info().monthlyCharge);
    }
}

static void ensureValidAmount(money_t amount) {
    if (amount <= 0)
        throw std::invalid_argument("Invalid amount");
}

void Account::charge(money_t amount) {
    addLog(ChargeMessage(clock_.date(), amount, Currency::ENC));
    balance_ -= amount;
}

void Account::capitalizeInterest() {
    money_t interest = (balance_ * info().interestRate + 5000) / 10000; //todo check rounding
    addLog(InterestMessage(clock_.date(), interest, currency()));
    balance_ += interest;
}

void Account::chargeForTransfer() {
    if (info().transferCharge != 0) {
        charge(info().transferCharge);
    }
}

void Account::addLog(const HistoryMessage& msg) {
    log_.addLog(msg);
}

void Account::print(std::ostream& os) const {
    os << "Account: " << id() << "\n";
    os << "balance: " << balance() << "\n";
    os << "history: " << "\n";
    os << history();
}

Account::~Account() {
    interstellarClock().unregisterObserver(*this);
}

Account::Account(InterstellarClock& clock, AccountRegistry& registry, const BankExchangeTable &exchangeTable, AccountInfo info)
    : clock_(clock), registry_(registry), balance_(0), exchangeTable_(exchangeTable), info_(info) {
    id_ = registry.generateId();
    registry.addAccount(*this);
    interstellarClock().registerObserver(*this);
}

SavingAccount::SavingAccount(InterstellarClock& clock, AccountRegistry& registry, const BankExchangeTable& exchangeTable, AccountInfo info)
    : Account(clock, registry, exchangeTable, info) {}

Currency SavingAccount::currency() const {
    return Currency::ENC;
}

void SavingAccount::transfer(double amountF, IdType to, std::string message) {
    money_t amount = doubleToMoney(amountF);
    ensureValidAmount(amount);
    registry_.getAccount(to).acceptTransfer(amount, id(), message);
    balance_ -= amount;
    addLog(TransferMessage(clock_.date(), -amount, Currency::ENC, id(), to, message));
    chargeForTransfer();
}

void SavingAccount::acceptTransfer(money_t amount, IdType from, std::string message) {
    addLog(TransferMessage(clock_.date(), amount, Currency::ENC, from, id(), message));
    balance_ += amount;
}

CurrencyAccount::CurrencyAccount(InterstellarClock& clock, AccountRegistry& registry,
                                 const BankExchangeTable& exchangeTable,
                                 AccountInfo info,
                                 Currency currency)
    : Account(clock, registry, exchangeTable, info), currency_(currency) {}

Currency CurrencyAccount::currency() const {
    return currency_;
}


void CurrencyAccount::deposit(double amountF) {
    return deposit({amountF, currency_});  // or maybe credits?
}

void CurrencyAccount::deposit(std::pair<double, Currency> amountInCurrency) {
    money_t amount = doubleToMoney(amountInCurrency.first);
    Currency currencyHere = amountInCurrency.second;
    ensureValidAmount(amount);
    if (currencyHere != Currency::ENC && currencyHere != currency_)
        throw std::invalid_argument("Invalid currency in deposit");

    addLog(DepositMessage(clock_.date(), amount, currencyHere));
    balance_ += exchangeTable_.exchangeSell(amount, currencyHere, currency_);
}

void CurrencyAccount::withdraw(double amountF) {
    return withdraw({amountF, currency_});  // or maybe credits?
}

void CurrencyAccount::withdraw(std::pair<double, Currency> amountInCurrency) {
    money_t amount = doubleToMoney(amountInCurrency.first);
    Currency currencyHere = amountInCurrency.second;
    ensureValidAmount(amount);
    if (currencyHere != Currency::ENC && currencyHere != currency_)
        throw std::invalid_argument("Invalid currency in withdraw");

    money_t convertedAmount = exchangeTable_.exchangeBuy(amount, currencyHere, currency_);
    if (balance_ < convertedAmount)
        throw NotEnoughMoney();
    addLog(WithdrawMessage(clock_.date(), amount, currencyHere));
    balance_ -= convertedAmount;
}

void CurrencyAccount::transfer(double amountF, IdType to, std::string message) {
    money_t amount = doubleToMoney(amountF);
    ensureValidAmount(amount);
    if (balance_ < amount)
        throw NotEnoughMoney();
    money_t encValue = exchangeTable_.exchangeSell(amount, currency_, Currency::ENC);
    registry_.getAccount(to).acceptTransfer(amount, id(), message);
    balance_ -= amount;
    addLog(TransferMessage(clock_.date(), -encValue, Currency::ENC, id(), to, message));
    chargeForTransfer();
}

void CurrencyAccount::acceptTransfer(money_t amount, IdType from, std::string message) {
    addLog(TransferMessage(clock_.date(), amount, Currency::ENC, from, id(), message));
    balance_ += exchangeTable_.exchangeBuy(amount, Currency::ENC, currency_);
}

void CurrencyAccount::charge(money_t amount) {
    addLog(ChargeMessage(clock_.date(), amount, currency()));
    balance_ -= exchangeTable_.exchangeBuy(amount, Currency::ENC, currency_);
}
