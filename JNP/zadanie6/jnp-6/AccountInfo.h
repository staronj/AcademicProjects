#ifndef _BANK_ACCOUNT_INFO_H
#define _BANK_ACCOUNT_INFO_H

#include "CurrencyTypes.h"

struct AccountInfo {
    money_t monthlyCharge = 0, transferCharge = 0;
    money_t interestRate = 0; /* here the type is slighly unnatural */
};

#endif  // _BANK_ACCOUNT_INFO_H
