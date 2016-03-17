#include "BankExchangeTable.h"
#include <exception>
#include <cmath>

BankExchangeTable& BankExchangeTable::exchangeRate(Currency currency)  {
    if (!isExchangableCurrency(currency)) {
        throw std::invalid_argument("Can't exchange for this currency");
    }
    lastCurrency_ = currency;
    return *this;
}

static void ensureRate(double rate) {
    if (!std::isfinite(rate) || rate <= 0)
        throw std::invalid_argument("Invalid rate");
}

BankExchangeTable& BankExchangeTable::buyingRate(double rate) {
    ensureRate(rate);
    if (lastCurrency_ == Currency::UNKNOWN)
        throw std::logic_error("Didn't set up currency type by exchangeRate");
    buyingRates_[static_cast<int>(lastCurrency_)] = doubleToExchange(rate);
    return *this;
}

BankExchangeTable& BankExchangeTable::sellingRate(double rate) {
    ensureRate(rate);
    if (lastCurrency_ == Currency::UNKNOWN)
        throw std::logic_error("Didn't set up currency type by exchangeRate");
    sellingRates_[static_cast<int>(lastCurrency_)] = doubleToExchange(rate);
    return *this;
}

money_t BankExchangeTable::buyENC(money_t amount, Currency currency) const {
    exchange_t exchangeRate = sellingRates_[static_cast<int>(currency)];
    //return (10000 * amount + (exchangeRate / 2)) / exchangeRate; // todo check rounding
    return (amount * exchangeRate + 5000) / 10000; // todo check rounding;
}

money_t BankExchangeTable::sellENC(money_t amount, Currency currency) const {
    exchange_t exchangeRate = buyingRates_[static_cast<int>(currency)];
    return (10000 * amount + (exchangeRate / 2)) / exchangeRate;
    //return (amount * exchangeRate + 5000) / 10000; // todo check rounding;
}

static money_t currencyToENC(money_t amount, Currency currency, const BankExchangeTable::ExchangeArray &rates) {
    exchange_t rate = rates[static_cast<int>(currency)];
    return (amount * rate + 5000) / 10000; // todo check rounding;
}

static money_t ENCToCurrency(money_t amount, Currency currency, const BankExchangeTable::ExchangeArray &rates) {
    exchange_t rate = rates[static_cast<int>(currency)];
    return (10000 * amount + (rate / 2)) / rate;
}

money_t BankExchangeTable::exchangeSell(money_t amount, Currency from, Currency to) const {
    if (from == to) {
        return amount;
    }
    return ENCToCurrency(currencyToENC(amount, from, buyingRates_), to, sellingRates_);
}

money_t BankExchangeTable::exchangeBuy(money_t amount, Currency from, Currency to) const {
    if (from == to) {
        return amount;
    }
    return ENCToCurrency(currencyToENC(amount, from, sellingRates_), to, buyingRates_);
}
