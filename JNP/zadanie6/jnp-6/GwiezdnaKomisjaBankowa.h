#ifndef _GWIEZDNA_KOMISJA_BANKOWA_H
#define _GWIEZDNA_KOMISJA_BANKOWA_H

#include "Bank.h"

#include "AccountRegistry.h"
#include "BankAccount.h"
#include "BankBuilder.h"
#include <map>
#include <memory>
#include <vector>

class GwiezdnaKomisjaBankowa {
    friend GwiezdnaKomisjaBankowa& gkb();
    GwiezdnaKomisjaBankowa() = default;
    GwiezdnaKomisjaBankowa(GwiezdnaKomisjaBankowa&&) = delete;
    GwiezdnaKomisjaBankowa(const GwiezdnaKomisjaBankowa&) = delete;

public:
    BankBuilder bankApplication() { return BankBuilder(accountRegistry_); }

    Bank& addBank(Bank &&bank);

    ObservableClock &clock() {
    	return clock_;
    }

private:
	ObservableClock clock_;
    AccountRegistry accountRegistry_;
    std::vector<std::unique_ptr<Bank> > banks_;
};

GwiezdnaKomisjaBankowa& gkb();
ObservableClock& interstellarClock();

#endif  // _GWIEZDNA_KOMISJA_BANKOWA_H
