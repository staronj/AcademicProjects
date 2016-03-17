#include "AccountRegistry.h"
#include "BankAccount.h"
#include <exception>

Account& AccountRegistry::getAccount(IdType accountId) const {
    auto it = accounts_.find(accountId);
    if (it == accounts_.end()) {
        throw std::out_of_range("Could not find account");
    }
    return *it->second;
}

void AccountRegistry::addAccount(Account& account) {
    auto it = accounts_.find(account.id());
    if (it != accounts_.end()) {
        throw std::logic_error("Adding account second time");
    }
    accounts_.emplace(account.id(), &account);
}
