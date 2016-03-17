#ifndef _CURRENCY_TYPES_H
#define _CURRENCY_TYPES_H

#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

enum class Currency {
  ENC, DIL, BIC, LIT, CurrencyCount_, UNKNOWN
};

inline bool isLegalCurrency(Currency c) {
  return c != Currency::CurrencyCount_ && c != Currency::UNKNOWN;
}

inline bool isExchangableCurrency(Currency c) {
  return isLegalCurrency(c) && c != Currency::ENC;
}

inline std::string currencyName(Currency c) {
  static std::string currencyNames[] = {
      "ENC", "DIL", "BIC", "LIT", "INVALID__CurrencyCount_", "INVALID__UNKNOWN"};
  return currencyNames[int(c)];
}

/* the type holding 100th parts of every currency */
using money_t = int64_t;

inline money_t doubleToMoney(double amount) {
  return static_cast<money_t>(round(amount * 100));
}

/* class for printing the money with currency */
class MoneyFormat {
  public:
  MoneyFormat(money_t amount, Currency currency) : amount_(amount), currency_(currency) { }

  private:
  money_t amount_;
  Currency currency_;

  friend std::ostream& operator<<(std::ostream& os, const MoneyFormat&);
};

inline std::ostream& operator<<(std::ostream& os, const MoneyFormat& moneyFormat) {
  bool sign = (moneyFormat.amount_ < 0);
  money_t amount = (moneyFormat.amount_ < 0 ? -moneyFormat.amount_ : moneyFormat.amount_);

  std::stringstream stream;
  stream.setf(std::ios_base::right);
  stream << (sign ? "-" : "") << amount / 100 << ".";
  stream << std::right << std::setw(2) << std::setfill('0') << amount % 100;
  stream << currencyName(moneyFormat.currency_);

  return os << stream.str();
}

#endif  // _CURRENCY_TYPES_H
