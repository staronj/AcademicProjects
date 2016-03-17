#include "Bank.h"
#include "AccountRegistry.h"
#include "BankAccount.h"
#include "Utils.h"
#include "GwiezdnaKomisjaBankowa.h"

Bank::Bank(AccountRegistry& accountRegistry,
           std::string name,
           AccountInfo checkingAccount,
           AccountInfo savingAccount,
           AccountInfo currencyAccount)
    : accountRegistry_(accountRegistry), name_(std::move(name)),
      checkingAccount_(std::move(checkingAccount)), savingAccount_(std::move(savingAccount)),
                                                                  currencyAccount_(std::move(currencyAccount))
{}

CurrencyAccount& Bank::openCheckingAccount(const Person& person) {
    return openAccount(make_unique<CurrencyAccount>(interstellarClock(), accountRegistry_, exchangeTable_, checkingAccount_, Currency::ENC));
}

SavingAccount& Bank::openSavingAccount(const Person& person) {
    return openAccount(make_unique<SavingAccount>(interstellarClock(), accountRegistry_, exchangeTable_, savingAccount_));
}

CurrencyAccount& Bank::openCurrencyAccount(const Person& person, Currency currency) {
    if (!isExchangableCurrency(currency)) {
        throw std::invalid_argument("openCurrencyAccount - invalid currency");
    }
    return openAccount(make_unique<CurrencyAccount>(interstellarClock(), accountRegistry_, exchangeTable_, currencyAccount_, currency));
}

template <class T>
T& Bank::openAccount(std::unique_ptr<T>&& account) {
    T &ref = *account;
    accounts.push_back(std::move(account));
    return ref;
}

BankExchangeTable& Bank::exchangeTable() {
    return exchangeTable_;
}
