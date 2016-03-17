#ifndef _BANK_H
#define _BANK_H

#include "AccountInfo.h"
#include "BankAccount.h"
#include "BankExchangeTable.h"
#include "CurrencyTypes.h"
#include "Person.h"
#include <map>
#include <memory>
#include <string>

class AccountRegistry;

class Bank {
public:
    CurrencyAccount& openCheckingAccount(const Person& person);
    SavingAccount& openSavingAccount(const Person& person);
    CurrencyAccount& openCurrencyAccount(const Person& person, Currency currency);
    BankExchangeTable& exchangeTable();
    Bank(Bank&&) = default;
    Bank(const Bank&) = delete;

private:
    template <class T>
    T& openAccount(std::unique_ptr<T>&& account);

    friend class BankBuilderImpl;

    Bank(AccountRegistry& accountRegistry,
         std::string name,
         AccountInfo checkingAccount,
         AccountInfo savingAccount,
         AccountInfo currencyAccount);

    AccountRegistry& accountRegistry_;

    std::vector<std::unique_ptr<Account>> accounts;
    std::string name_;
    BankExchangeTable exchangeTable_;
    AccountInfo checkingAccount_, savingAccount_, currencyAccount_;
};

#endif  // _BANK_H
