#include "gsb.h"
#include <iostream>
#include <cassert>

int main() {
  {
    auto& simple_bank = gkb().bankApplication().name("Bank").createBank();
    auto& andrzej = earth().registerCitizen("Andrzej");
    auto& checking = simple_bank.openCheckingAccount(andrzej);
  }

  {
    auto& simple_bank = gkb().bankApplication().name("Bank").createBank();
    auto& andrzej = earth().registerCitizen("Andrzej");
    auto& saving = simple_bank.openSavingAccount(andrzej);
  }

  {
    auto& simple_bank = gkb().bankApplication().name("Bank").createBank();
    auto& andrzej = earth().registerCitizen("Andrzej");
    auto& account_bic = simple_bank.openCurrencyAccount(andrzej, Currency::BIC);
    auto& account_dil = simple_bank.openCurrencyAccount(andrzej, Currency::DIL);
    auto& account_lit = simple_bank.openCurrencyAccount(andrzej, Currency::LIT);

    // trying to set buying rate of unknown currency is illegal
    try {
      simple_bank.exchangeTable().buyingRate(2.0);
      assert(false);
    }
    catch (...){
    }

    // trying to set buying rate of unknown currency is illegal
    try {
      simple_bank.exchangeTable().sellingRate(2.0);
      assert(false);
    }
    catch (...){
    }

  }
  return 0;
}

