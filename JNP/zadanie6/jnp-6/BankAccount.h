#ifndef _BANK_ACCOUNT_H
#define _BANK_ACCOUNT_H

#include "CurrencyTypes.h"
#include "IdType.h"
#include "InterstellarClock.h"
#include "Person.h"

#include "AccountInfo.h"
#include "BankExchangeTable.h"
#include "AccountRegistry.h"
#include "History.h"
#include <iomanip>
#include <ostream>
#include <string>

class AccountRegistry;

class NotEnoughMoney : public std::exception {
  public:
  NotEnoughMoney() = default;
  const char* what() const noexcept override { return "not enough money"; }
};

class Account : public InterstellarClockObserver {
public:
    IdType id() const;
    virtual void transfer(double amountF, IdType id, std::string message = "") = 0;
    money_t balanceNumber() const;
    std::string balance() const;
    std::string history() const;
    virtual void acceptTransfer(money_t amount, IdType from, std::string title) = 0;
    virtual Currency currency() const = 0;

    virtual ~Account();

protected:
    Account(InterstellarClock& clock, AccountRegistry& registry, const BankExchangeTable &exchangeTable, AccountInfo info);

    const AccountInfo& info() { return info_; }

    virtual void print(std::ostream& os) const;

    void onEveryMonth() override;

    virtual void charge(money_t amount);
    virtual void capitalizeInterest();
    virtual void chargeForTransfer();

    void addLog(const HistoryMessage& msg);

    const InterstellarClock &clock_;
    const AccountRegistry &registry_;
    money_t balance_;
    const BankExchangeTable &exchangeTable_;
private:
    IdType id_;
    HistoryLog log_;
    AccountInfo info_;
    friend std::ostream& operator<<(std::ostream& os, const Account&);
};

inline std::ostream& operator<<(std::ostream& os, const Account& account) {
    account.print(os);
    return os;
}

class SavingAccount : public Account {
public:
    SavingAccount(InterstellarClock& clock, AccountRegistry& registry, const BankExchangeTable &exchangeTable, AccountInfo info);
    Currency currency() const override;
    void transfer(double amountF, IdType id, std::string message = "") override;
    void acceptTransfer(money_t amount, IdType from, std::string title) override;
};

class CurrencyAccount : public Account {
public:
    CurrencyAccount(InterstellarClock& clock, AccountRegistry& registry,
                    const BankExchangeTable &exchangeTable,
                    AccountInfo info,
                    Currency currency);
    Currency currency() const override;
    void deposit(double amountF);
    void deposit(std::pair<double, Currency> amountInCurrency);
    void withdraw(double amountF);
    void withdraw(std::pair<double, Currency> amountInCurrency);
    void transfer(double amountF, IdType to, std::string message = "") override;
    void acceptTransfer(money_t amount, IdType from, std::string title) override;
protected:
    void charge(money_t amount) override;
private:
    Currency currency_;
};

#endif  // _BANK_ACCOUNT_H
