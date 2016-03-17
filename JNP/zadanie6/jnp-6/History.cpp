#include "History.h"
#include "BankAccount.h"

void HistoryLog::addLog(const HistoryMessage& hm) {
    std::stringstream stream;
    stream << hm << std::endl;
    log_ += stream.str();
}

std::ostream& operator<<(std::ostream& os, const HistoryMessage& hm) {
    hm.print(os);
    return os;
}

TransferMessage::TransferMessage(Date day, money_t howMuch, Currency currency, IdType from, IdType to, std::string message) :
    day_(day),
    howMuch_(howMuch),
    currency_(currency),
    from_(from),
    to_(to),
    message_(message) { }

void TransferMessage::print(std::ostream& os) const {
    os << day_ << ' ' << MoneyFormat(howMuch_, currency_);
    os << " TRANSFER " << "(" << message_ << ")" << " FROM: ";
    os << from_ << " TO: " << to_;
}

DepositMessage::DepositMessage(Date day, money_t howMuch, Currency currency) :
    day_(day),
    howMuch_(howMuch),
    currency_(currency) { }

void DepositMessage::print(std::ostream& os) const {
    os << day_ << ' ' << MoneyFormat(howMuch_, currency_);
    os << " DEPOSIT";
}

WithdrawMessage::WithdrawMessage(Date day, money_t howMuch, Currency currency) :
    day_(day),
    howMuch_(howMuch),
    currency_(currency) { }

void WithdrawMessage::print(std::ostream& os) const {
    os << day_ << ' ' << '-' << MoneyFormat(howMuch_, currency_);
    os << " WITHDRAWAL";
}

ChargeMessage::ChargeMessage(Date day, money_t howMuch, Currency currency) :
    day_(day),
    howMuch_(howMuch),
    currency_(currency) { }

void ChargeMessage::print(std::ostream& os) const {
    os << day_ << ' ' << '-' << MoneyFormat(howMuch_, currency_);
    os << " CHARGE";
}

InterestMessage::InterestMessage(Date day, money_t howMuch, Currency currency) :
    day_(day),
    howMuch_(howMuch),
    currency_(currency) { }

void InterestMessage::print(std::ostream& os) const {
    os << day_ << ' ' << MoneyFormat(howMuch_, currency_);
    os << " INTEREST";
}
