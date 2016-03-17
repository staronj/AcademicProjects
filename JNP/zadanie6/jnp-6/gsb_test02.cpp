#include "gsb.h"
#include <bits/stdc++.h>

using namespace std;

string get_funny_string() {
    string s = "hello";
    s += string(1, '\0');
    s += "world";

    return s;
}

#define DO_EXCEPTION(op, testid)                                                    \
    do {                                                                            \
        try {                                                                       \
            op;                                                                     \
            cerr << "Not caught: " #op "(" << testid << ") @ " << __LINE__ << endl; \
            assert(false);                                                          \
        } catch (std::exception e) {                                                \
            cout << "OK " << testid << endl;                                        \
        }                                                                           \
    } while (0)

int main() {
    auto& C1 = earth().registerCitizen("aosidbiuasdbc13421");
    auto& C2 = qonos().registerCitizen("soivadsf nasd dsaioufh sdd asoudf adsfahnsf df ");
    auto& C3 = bynaus().registerCitizen(get_funny_string());
    auto& C4 = bynaus().registerCitizen("Hello!");
    auto& C5 = earth().registerCitizen("1234567890!@#$%^&*()-=_+[]\\{}|;':\",./<>?`~");
    auto& C6 = bynaus().registerCitizen(C3, C4);
    auto& C7 = bynaus().registerCitizen(C6, C3);
    auto& C8 = C4;

#ifdef NCOMPILE101
    bynaus().registerCitizen(C1, C2);
#endif

#ifdef NCOMPILE102
    bynaus().registerCitizen(C7, C5);
#endif

#ifdef NCOMPILE103
    bynaus().registerCitizen(C4, C2);
#endif

#ifdef NCOMPILE104
    bynaus().registerCitizen(C1, C3);
#endif

    cout << "C1: " << C1.name() << " ||| " << C1.id() << "\n"
        << "C2: " << C2.name() << " ||| " << C2.id() << "\n"
        << "C3: " << C3.name() << " ||| " << C3.id() << "\n"
        << "C4: " << C4.name() << " ||| " << C4.id() << "\n"
        << "C5: " << C5.name() << " ||| " << C5.id() << "\n"
        << "C6: " << C6.name() << " ||| " << C6.id() << "\n"
        << "C7: " << C7.name() << " ||| " << C7.id() << "\n"
        << "C8: " << C8.name() << " ||| " << C8.id() << "\n\n";

    /*assert(!(C1 == C2));
    assert(!(C6 == C7));
    assert(C4 == C8);
    assert(C2 != C5);
    assert(!(C8 != C4));*/

    auto& g = gkb();
    auto&& app = g.bankApplication().checkingAccount();

    DO_EXCEPTION(app.monthlyCharge(-44), 101);
    DO_EXCEPTION(app.monthlyCharge(-1. / 0.), 102);
    DO_EXCEPTION(app.monthlyCharge(1. / 0.), 103);
    DO_EXCEPTION(app.monthlyCharge(nan("")), 104);

    DO_EXCEPTION(app.transferCharge(-44), 111);
    DO_EXCEPTION(app.transferCharge(-1. / 0.), 112);
    DO_EXCEPTION(app.transferCharge(1. / 0.), 113);
    DO_EXCEPTION(app.transferCharge(nan("")), 114);

    //DO_EXCEPTION(app.interestRate(-44), 121);
    DO_EXCEPTION(app.interestRate(-1. / 0.), 122);
    DO_EXCEPTION(app.interestRate(1. / 0.), 123);
    DO_EXCEPTION(app.interestRate(nan("")), 124);

    auto& B1 = app.monthlyCharge(50).name(string("hello")).createBank();
    auto& B2 = gkb().bankApplication().checkingAccount()
        .name("world").transferCharge(15).createBank();

    auto& E1 = B1.exchangeTable();

    DO_EXCEPTION(E1.exchangeRate(Currency::ENC), 131);
    E1.exchangeRate(Currency::DIL);

    DO_EXCEPTION(E1.buyingRate(0), 132);
    DO_EXCEPTION(E1.buyingRate(-44), 133);
    DO_EXCEPTION(E1.buyingRate(1. / 0.), 134);
    DO_EXCEPTION(E1.buyingRate(nan("")), 135);
    DO_EXCEPTION(E1.sellingRate(0), 136);
    DO_EXCEPTION(E1.sellingRate(-44), 137);
    DO_EXCEPTION(E1.sellingRate(1. / 0.), 138);
    DO_EXCEPTION(E1.sellingRate(nan("")), 139);

    auto& A1 = B2.openCheckingAccount(C5);
    auto& A2 = B1.openCheckingAccount(C1);

    A1.deposit(100);
    DO_EXCEPTION(A1.transfer(5, "blablabla"), 151);
    DO_EXCEPTION(A1.transfer(100.01, A2.id(), "duzo to"), 152);
    {
        std::stringstream ss;
        ss << A1.balance();
        cout << A1.balance() << "\n";
        assert(ss.str() == "100.00ENC");
    }

    A1.transfer(85.01, A2.id());
    {
        std::stringstream ss;
        ss << A1.balance() << "|" << A2.balance();
        assert(ss.str() == "-0.01ENC|85.01ENC");
    }

    A2.transfer(85.01, A1.id());

    {
        std::stringstream ss;
        ss << A1.balance() << "|" << A2.balance();
        assert(ss.str() == "85.00ENC|0.00ENC");
    }

    DO_EXCEPTION(A1.transfer(-500000, A2.id(), "ALL YOUR MONIES ARE BELONG TO US"), 154);
    DO_EXCEPTION(A1.transfer(0, A2.id(), "heheszki"), 155);
    DO_EXCEPTION(A1.transfer(1. / 0., A2.id()), 156);
    DO_EXCEPTION(A1.transfer(nan("aa"), A2.id()), 157);

    DO_EXCEPTION(A1.deposit(1. / 0.), 161);
    DO_EXCEPTION(A1.deposit(0), 162);
    DO_EXCEPTION(A1.deposit(-44), 163);
    DO_EXCEPTION(A1.deposit(nan("")), 164);

    DO_EXCEPTION(A1.withdraw(1. / 0.), 165);
    DO_EXCEPTION(A1.withdraw(85.01), 166);
    DO_EXCEPTION(A1.withdraw(0), 167);
    DO_EXCEPTION(A1.withdraw(-15), 168);
    DO_EXCEPTION(A1.withdraw(nan("")), 169);

    DO_EXCEPTION(A1.deposit({1. / 0., Currency::ENC}), 170);
    DO_EXCEPTION(A1.deposit({0, Currency::ENC}), 171);
    DO_EXCEPTION(A1.deposit({-44, Currency::ENC}), 172);
    DO_EXCEPTION(A1.deposit({nan(""), Currency::ENC}), 173);

    DO_EXCEPTION(A1.withdraw({1. / 0., Currency::ENC}), 174);
    DO_EXCEPTION(A1.withdraw({0, Currency::ENC}), 175);
    DO_EXCEPTION(A1.withdraw({85.02, Currency::ENC}), 176);
    DO_EXCEPTION(A1.withdraw({-15, Currency::ENC}), 177);
    DO_EXCEPTION(A1.withdraw({nan(""), Currency::ENC}), 178);

    DO_EXCEPTION(A1.deposit({5, Currency::LIT}), 179);
    DO_EXCEPTION(A1.deposit({5, Currency::DIL}), 180);
    DO_EXCEPTION(A1.deposit({5, Currency::BIC}), 181);
    DO_EXCEPTION(A1.withdraw({5, Currency::LIT}), 182);
    DO_EXCEPTION(A1.withdraw({5, Currency::DIL}), 183);
    DO_EXCEPTION(A1.withdraw({5, Currency::BIC}), 184);

    auto& AX = B2.openCurrencyAccount(C5, Currency::LIT);
    auto& AY = B2.openSavingAccount(C5);

    DO_EXCEPTION(AX.deposit({100, Currency::DIL}), 185);
    DO_EXCEPTION(AX.deposit({100, Currency::BIC}), 186);
    AX.deposit(10000);
    DO_EXCEPTION(AX.withdraw({100, Currency::DIL}), 187);
    DO_EXCEPTION(AX.withdraw({100, Currency::BIC}), 188);

    return 0;
}
