#ifndef _BANK_BUILDER_H
#define _BANK_BUILDER_H

#include "AccountInfo.h"
#include "Bank.h"

#include <string>
#include <memory>

class AccountRegistry;

class BankBuilderImpl {
public:
    BankBuilderImpl(AccountRegistry& accountRegistry) :
      used_once_(false), accountRegistry_(accountRegistry), lastAccount_(nullptr) { }
    void name(std::string name);
    void checkingAccount();
    void savingAccount();
    void monthlyCharge(double charge);
    void transferCharge(double charge);
    void interestRate(double rate);
    void currencyAccount();
    Bank& createBank();

private:
    void ensureBuilderNotUsed();
    void ensureLastAccountInitialized();

    bool used_once_;
    AccountRegistry& accountRegistry_;
    std::string bankName_ = "???";
    AccountInfo* lastAccount_;
    AccountInfo checkingAccount_, savingAccount_, currencyAccount_;
};

class BankBuilder {
public:
    BankBuilder(AccountRegistry& accountRegistry) :
      internal(std::make_shared<BankBuilderImpl>(accountRegistry)) {}
    BankBuilder name(std::string name) {
        internal->name(name);
        return *this;
    }
    BankBuilder checkingAccount() {
        internal->checkingAccount();
        return *this;
    }
    BankBuilder savingAccount() {
        internal->savingAccount();
        return *this;
    }
    BankBuilder monthlyCharge(double charge) {
        internal->monthlyCharge(charge);
        return *this;
    }
    BankBuilder transferCharge(double charge) {
        internal->transferCharge(charge);
        return *this;
    }
    BankBuilder interestRate(double rate) {
        internal->interestRate(rate);
        return *this;
    }
    BankBuilder currencyAccount() {
        internal->currencyAccount();
        return *this;
    }
    Bank& createBank() {
        return internal->createBank();
    }

private:
    std::shared_ptr<BankBuilderImpl> internal;
};

#endif  // _BANK_BUILDER_H
