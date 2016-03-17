#ifndef _ACCOUNT_REGISTRY_H
#define _ACCOUNT_REGISTRY_H

#include "IdType.h"
#include <map>

class Account;

class AccountRegistry {
public:
    Account& getAccount(IdType accountId) const;
    IdType generateId() { return IdType(nextAccountId_++); };
    void addAccount(Account& account);

private:
    int nextAccountId_;
    std::map<IdType, Account*> accounts_;
};

#endif  //_ACCOUNT_REGISTRY_H
