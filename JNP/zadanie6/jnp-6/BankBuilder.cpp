#include "BankBuilder.h"
#include "GwiezdnaKomisjaBankowa.h"
#include <cmath>
#include <exception>

static void ensureValidCharge(double charge) {
    if (!std::isfinite(charge) || charge < 0)
        throw std::invalid_argument("Invalid charge");
}

void BankBuilderImpl::name(std::string name) {
    ensureBuilderNotUsed();
    bankName_ = std::move(name);
}
void BankBuilderImpl::checkingAccount() {
    ensureBuilderNotUsed();
    lastAccount_ = &checkingAccount_;
}
void BankBuilderImpl::savingAccount() {
    ensureBuilderNotUsed();
    lastAccount_ = &savingAccount_;
}
void BankBuilderImpl::currencyAccount() {
    ensureBuilderNotUsed();
    lastAccount_ = &currencyAccount_;
}
void BankBuilderImpl::monthlyCharge(double charge) {
    ensureBuilderNotUsed();
    ensureLastAccountInitialized();
    ensureValidCharge(charge);
    lastAccount_->monthlyCharge = doubleToMoney(charge);
}
void BankBuilderImpl::transferCharge(double charge) {
    ensureBuilderNotUsed();
    ensureLastAccountInitialized();
    ensureValidCharge(charge);
    lastAccount_->transferCharge = doubleToMoney(charge);
}

void BankBuilderImpl::interestRate(double rate) {
    ensureBuilderNotUsed();
    ensureLastAccountInitialized();
    ensureValidCharge(rate);
    lastAccount_->interestRate = doubleToMoney(rate);
}
Bank& BankBuilderImpl::createBank() {
    if (used_once_)
        throw std::runtime_error("createBank used more than once");
    used_once_ = true;
    return gkb().addBank(Bank(accountRegistry_,
                              std::move(bankName_),
                              std::move(checkingAccount_),
                              std::move(savingAccount_),
                              std::move(currencyAccount_)));
}

void BankBuilderImpl::ensureBuilderNotUsed() {
    if (used_once_)
        throw std::logic_error("used more than once!");
}
void BankBuilderImpl::ensureLastAccountInitialized() {
    if (!lastAccount_)
        throw std::logic_error("Must call *Account() method");
}
