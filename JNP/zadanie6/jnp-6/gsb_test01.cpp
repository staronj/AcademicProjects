#include "gsb.h"
#include <bits/stdc++.h>

using namespace std;

int main() {
    auto& C1 = earth().registerCitizen("C1");
    auto& C2 = qonos().registerCitizen("C2");
    auto& C3 = bynaus().registerCitizen("C3");

    /* Hm, co sie powinno stac, jak polaczymy C3 z C3? */
    /* W sumie nie wiem. Poki co zakladam, ze dziala. */
    auto& C4 = bynaus().registerCitizen(C3, C3);


    auto& someBank = gkb().bankApplication()
        .name("someBank")
        .checkingAccount().monthlyCharge(3.14).transferCharge(2.72).interestRate(1.31)
        .savingAccount().monthlyCharge(1.62).transferCharge(0.58).interestRate(0.62)
        .currencyAccount().monthlyCharge(1.41).transferCharge(1.20).interestRate(1.73)
        .createBank();

    auto& someOtherBank = gkb().bankApplication()
        .name("someOtherBank")
        .savingAccount().monthlyCharge(5).monthlyCharge(4).monthlyCharge(3)
                        .interestRate(5).monthlyCharge(7).interestRate(3)
        .createBank();


    auto& A1 = someBank.openCheckingAccount(C1);
    auto& A2 = someBank.openSavingAccount(C2);
    auto& A3 = someBank.openCheckingAccount(C3);
    auto& A4 = someBank.openCurrencyAccount(C1, Currency::LIT);
    auto& A5 = someOtherBank.openCurrencyAccount(C4, Currency::BIC);
    auto& A6 = someBank.openCurrencyAccount(C2, Currency::BIC);
    auto& A7 = someBank.openCurrencyAccount(C3, Currency::DIL);

    try {
        someBank.openCurrencyAccount(C3, Currency::ENC);
        cout << "ERROR 1" << endl;
        return 1;
    } catch(std::exception e) {}


    cout << "A1 = " << A1.id() << "\n"
         << "A2 = " << A2.id() << "\n"
         << "A3 = " << A3.id() << "\n"
         << "A4 = " << A4.id() << "\n"
         << "A5 = " << A5.id() << "\n"
         << "A6 = " << A6.id() << "\n"
         << "A7 = " << A7.id() << "\n\n";

    A1.deposit(12345);
    A3.deposit(4321);
    A4.deposit(5555);
    A5.deposit({4444, Currency::ENC});
    A6.deposit({3333, Currency::BIC});

    const int M = 10000;

    A1.transfer(M, A2.id(), "blahblah");

    someBank.exchangeTable()
        .exchangeRate(Currency::DIL).buyingRate(3.33).sellingRate(4.44).sellingRate(4.56)
        .exchangeRate(Currency::DIL)
        .exchangeRate(Currency::BIC).buyingRate(1.28).sellingRate(1.44)
        .exchangeRate(Currency::LIT).buyingRate(0.28).sellingRate(0.33);
    
    someOtherBank.exchangeTable()
        .exchangeRate(Currency::LIT).buyingRate(0.19).sellingRate(0.22)
        .exchangeRate(Currency::DIL).buyingRate(2.89).sellingRate(4.29)
        .exchangeRate(Currency::BIC).buyingRate(1.2).sellingRate(1.5);

    
    A2.transfer(1400, A3.id(), "are");
    A3.transfer(1500, A4.id(), "you");
    A4.transfer(1600, A5.id(), "fucking");
    interstellarClock().nextDay().nextDay().nextDay();
    A5.transfer(1700, A6.id(), "kidding");
    interstellarClock().nextDay().nextDay().nextDay();
    A6.transfer(1800, A1.id(), "me");

    interstellarClock().nextMonth().nextDay().nextDay().nextDay().nextDay();

    A1.transfer(555, A6.id());
    interstellarClock().nextDay().nextDay().nextDay();
    A6.transfer(666, A5.id());
    A5.transfer(777, A4.id());
    for (int i = 0; i < 8; i++) {
        interstellarClock().nextDay().nextDay().nextDay();
    }
    A4.transfer(888, A3.id());
    A3.transfer(999, A2.id());
    for (int i = 0; i < 12; i++) {
        interstellarClock().nextDay().nextDay().nextDay().nextDay();
    }
    A2.transfer(1110, A1.id());


    someBank.exchangeTable().exchangeRate(Currency::BIC).buyingRate(1.22);

    A1.deposit(M);
    A3.deposit(M);
    A4.deposit(M); A4.deposit({M, Currency::LIT}); A4.deposit({M, Currency::ENC});
    A5.deposit(M); A5.deposit({M, Currency::BIC}); A5.deposit({M, Currency::ENC});
    A6.deposit(M); A6.deposit({M, Currency::BIC}); A6.deposit({M, Currency::ENC});

    for (int i = 0; i < 120; i++) {
        interstellarClock().nextDay().nextDay().nextDay().nextDay();
    }

    A1.withdraw(M);
    A4.withdraw(M); A4.withdraw({M, Currency::LIT}); A4.withdraw({M, Currency::ENC});
    A5.withdraw(M); A5.withdraw({M, Currency::BIC}); A5.withdraw({M, Currency::ENC});
    A6.withdraw(M); A6.withdraw({M, Currency::BIC}); A6.withdraw({M, Currency::ENC});

    cout << A1 << endl;
    cout << A2 << endl;
    cout << A3 << endl;
    cout << A4 << endl;
    cout << A5 << endl;
    cout << A6 << endl;
    cout << A7 << endl;

    return 0;
}
