#ifndef _HISTORY_H
#define _HISTORY_H

#include <iostream>
#include <sstream>

#include "InterstellarClock.h"
#include "CurrencyTypes.h"
#include "IdType.h"

class HistoryMessage {
    // z tego dziedziczymy różne rodzaje wiadomości
protected:
    virtual void print(std::ostream& os) const = 0;
    friend std::ostream& operator<<(std::ostream& os, const HistoryMessage&);
};

std::ostream& operator<<(std::ostream& os, const HistoryMessage& hm);

class TransferMessage : public HistoryMessage {
  public:
  TransferMessage(Date day, money_t howMuch, Currency currency, IdType from, IdType to, std::string message="");

  protected:
  void print(std::ostream& os) const override;
  private:
  Date day_;
  money_t howMuch_;
  Currency currency_;
  IdType from_;
  IdType to_;
  std::string message_;
};

class DepositMessage : public HistoryMessage {
  public:
  DepositMessage(Date day, money_t howMuch, Currency currency);

  protected:
  void print(std::ostream& os) const override;
  private:
  Date day_;
  money_t howMuch_;
  Currency currency_;
};

class WithdrawMessage : public HistoryMessage {
  public:
  WithdrawMessage(Date day, money_t howMuch, Currency currency);

  protected:
  void print(std::ostream& os) const override;
  private:
  Date day_;
  money_t howMuch_;
  Currency currency_;
};

class ChargeMessage : public HistoryMessage {
  public:
  ChargeMessage(Date day, money_t howMuch, Currency currency);

  protected:
  void print(std::ostream& os) const override;
  private:
  Date day_;
  money_t howMuch_;
  Currency currency_;
};

class InterestMessage : public HistoryMessage {
  public:
  InterestMessage(Date day, money_t howMuch, Currency currency);

  protected:
  void print(std::ostream& os) const override;
  private:
  Date day_;
  money_t howMuch_;
  Currency currency_;
};

class HistoryLog {
public:
    void addLog(const HistoryMessage& hm);
    operator std::string() const { return log_; }

private:
    std::string log_;
};

#endif  // _HISTORY_H
