#include "GwiezdnaKomisjaBankowa.h"
#include "Utils.h"

Bank& GwiezdnaKomisjaBankowa::addBank(Bank&& bank) {
    banks_.push_back(make_unique<Bank>(std::move(bank)));
    return *banks_.back();
}

GwiezdnaKomisjaBankowa& gkb() {
    static GwiezdnaKomisjaBankowa singleton;
    return singleton;
}

ObservableClock& interstellarClock() {
    return gkb().clock();
}
