#ifndef _BANK_EXCHANGE_TABLE_H
#define _BANK_EXCHANGE_TABLE_H

#include "CurrencyTypes.h"
#include <array>

using exchange_t = int64_t;

inline exchange_t doubleToExchange(double x) {
    return static_cast<exchange_t>(round(10000 * x));
}

class BankExchangeTable {
public:
    using ExchangeArray = std::array<exchange_t, static_cast<int>(Currency::CurrencyCount_)>;
    BankExchangeTable& exchangeRate(Currency currency);
    BankExchangeTable& buyingRate(double rate);
    BankExchangeTable& sellingRate(double rate);

    money_t exchangeSell(money_t amount, Currency from, Currency to) const;
    money_t exchangeBuy(money_t amount, Currency from, Currency to) const;
private:
    money_t buyENC(money_t amount, Currency currency) const;
    money_t sellENC(money_t amount, Currency currency) const;

    Currency lastCurrency_ = Currency::UNKNOWN;
    // rate from currency -> ENC
    ExchangeArray buyingRates_ = {{
            doubleToExchange(1),
            doubleToExchange(1),
            doubleToExchange(1),
            doubleToExchange(1)
        }};
    // rate from ENC -> currency
    ExchangeArray sellingRates_ = {{
            doubleToExchange(1),
            doubleToExchange(1),
            doubleToExchange(1),
            doubleToExchange(1)
        }};
};

#endif  // _BANK_EXCHANGE_TABLE_H
