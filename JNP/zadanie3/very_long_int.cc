#include "very_long_int.h"
#include <algorithm>
#include <stdexcept>
#include <assert.h>

using namespace std;
typedef VeryLongInt::value_type value_type;

namespace {
// calculates floor of binary logarithm
size_t Log2(value_type n) {
  size_t result = 0;
  while (n > 0) {
    result++;
    n /= 2;
  }
  return result;
}
}

VeryLongInt::VeryLongInt() :
    data(new container_type()) { }

VeryLongInt::VeryLongInt(const VeryLongInt& model) {
  if (model.isValid())
    data.reset(new container_type(*model.data));
}

VeryLongInt::VeryLongInt(VeryLongInt&& model) : data(std::move(model.data)) { }

VeryLongInt::VeryLongInt(const std::string& number) {
  init_from_string(number);
}

VeryLongInt::VeryLongInt(const char * number) {
  if (number == nullptr) {
    setNaN();
    return;
  }
  init_from_string(number);
}

VeryLongInt& VeryLongInt::operator=(const VeryLongInt& model) {
  if (this == &model)
    return *this;

  if (model.isValid())
    data.reset(new container_type(*model.data));
  else
    setNaN();

  return *this;
}

VeryLongInt& VeryLongInt::operator=(VeryLongInt&& model) {
  if (this == &model)
    return *this;
  data = std::move(model.data);
  return *this;
}

int VeryLongInt::numberOfBinaryDigits() const {
  if (!isValid())
    return 0;
  else if (isZero())
    return 1;

  size_t result = base_digits_number * (length() - 1);
  result += Log2(data->back());

  return (int)result;
}

bool VeryLongInt::isValid() const {
  return data.get() != nullptr;
}

bool VeryLongInt::isZero() const {
  return isValid() && data->empty();
}

const VeryLongInt operator+(const VeryLongInt& a, const VeryLongInt& b) {
  VeryLongInt result = a;
  result += b;
  return result;
}

const VeryLongInt operator-(const VeryLongInt& a, const VeryLongInt& b) {
  VeryLongInt result = a;
  result -= b;
  return result;
}

const VeryLongInt operator*(const VeryLongInt& a, const VeryLongInt& b) {
  if (!a.isValid() || !b.isValid())
    return NaN();

  const size_t length = a.length() + b.length() + 1;
  VeryLongInt result;
  result.resize(length);

  for (size_t i = 0; i < a.length(); i++) {
    for (size_t j = 0; j < b.length(); j++)
      result.at(i + j) += a.at(i) * b.at(j);

    value_type carry = 0;
    for (size_t j = i; j < i + b.length() || carry > 0; j++) {
      result.at(j) += carry;
      carry = result.at(j) / VeryLongInt::base;
      result.at(j) %= VeryLongInt::base;
    }
  }

  result.normalize();
  return result;
}

const VeryLongInt operator/(const VeryLongInt& a, const VeryLongInt& b) {
  if (!a.isValid() || !b.isValid() || b.isZero())
    return NaN();

  return VeryLongInt::divide_modulo_impl(a, b).first;
}

const VeryLongInt operator%(const VeryLongInt& a, const VeryLongInt& b) {
  if (!a.isValid() || !b.isValid() || b.isZero())
    return NaN();

  return VeryLongInt::divide_modulo_impl(a, b).second;
}

VeryLongInt& VeryLongInt::operator+=(const VeryLongInt& b) {
  if (!isValid() || !b.isValid()) {
    setNaN();
  }
  else {
    size_t max_length = std::max(length(), b.length()) + 1;
    resize(max_length);
    for (size_t i = 0; i < b.length(); i++)
      at(i) += b.at(i);

    update_carry();
    normalize();
  }
  return *this;
}

VeryLongInt& VeryLongInt::operator-=(const VeryLongInt& b) {
  if (!isValid() || !b.isValid() || *this < b) {
    setNaN();
  }
  else {
    for (size_t i = 1 ; i < length() ; i++) {
      size_t index = length() - i - 1;
      at(index + 1)--;
      at(index) += base;
    }

    for (size_t i = 0; i < b.length(); i++)
      at(i) -= b.at(i);

    update_carry();
    normalize();
  }
  return *this;
}

VeryLongInt& VeryLongInt::operator*=(const VeryLongInt& b) {
  *this = std::move(*this * b);
  return *this;
}

VeryLongInt& VeryLongInt::operator/=(const VeryLongInt& b) {
  *this = std::move(*this / b);
  return *this;
}

VeryLongInt& VeryLongInt::operator%=(const VeryLongInt& b) {
  *this = std::move(*this % b);
  return *this;
}

VeryLongInt& VeryLongInt::operator<<=(unsigned int n) {
  *this = std::move(*this << n);
  return *this;
}

VeryLongInt& VeryLongInt::operator>>=(unsigned int n) {
  *this = std::move(*this >> n);
  return *this;
}

const VeryLongInt VeryLongInt::operator<<(unsigned int n) const {
  if (!isValid())
    return *this;

  const size_t block_shift = n / base_digits_number;
  const size_t small_shift = n % base_digits_number;

  VeryLongInt result;
  result.resize(length() + block_shift + 1);

  for (size_t i = 0; i < length(); i++)
    result.at(i + block_shift) = at(i) << small_shift;

  result.update_carry();
  result.normalize();

  return result;
}

const VeryLongInt VeryLongInt::operator>>(unsigned int n) const {
  if (!isValid())
    return *this;

  const size_t block_shift = n / base_digits_number;
  const size_t small_shift = n % base_digits_number;

  if (block_shift >= length()) {
    return Zero();
  }

  VeryLongInt result;
  result.resize(length() - block_shift);

  for (size_t i = 0; i < result.length(); i++)
    result.at(i) = at(i + block_shift);

  for (size_t i = 0; i < result.length(); i++) {
    result.at(i) >>= small_shift;
    if (i + 1 < result.length()) {
      value_type mask = (1u << small_shift) - 1;
      value_type carry = mask & result.at(i + 1);
      result.at(i) += carry << (base_digits_number - small_shift);
    }
  }

  result.update_carry();
  result.normalize();

  return result;
}

bool operator==(const VeryLongInt& a, const VeryLongInt& b) {
  if (!a.isValid() || !b.isValid())
    return false;

  if (a.length() != b.length())
    return false;

  return *(a.data) == *(b.data);
}

bool operator!=(const VeryLongInt& a, const VeryLongInt& b) {
  if (!a.isValid() || !b.isValid())
    return false;
  return !(a == b);
}

bool operator<(const VeryLongInt& a, const VeryLongInt& b) {
  if (!a.isValid() || !b.isValid())
    return false;

  if (a.length() != b.length())
    return a.length() < b.length();

  const size_t length = a.length();
  for (size_t i = 0; i < length; i++) {
    const size_t index = length - i - 1;
    if (a.at(index) != b.at(index))
      return a.at(index) < b.at(index);
  }

  return false;
}

bool operator<=(const VeryLongInt& a, const VeryLongInt& b) {
  if (!a.isValid() || !b.isValid())
    return false;
  return !(b < a);
}

bool operator>(const VeryLongInt& a, const VeryLongInt& b) {
  if (!a.isValid() || !b.isValid())
    return false;
  return b < a;
}

bool operator>=(const VeryLongInt& a, const VeryLongInt& b) {
  if (!a.isValid() || !b.isValid())
    return false;
  return b <= a;
}

VeryLongInt::operator bool() const {
  return isValid() && !isZero();
}

uint32_t VeryLongInt::toUint32() const {
  if (!isValid()) {
    throw std::invalid_argument("VeryLongInt::toUint32 - number is NaN");
  }
  else if (length() > 1) {
    throw std::invalid_argument("VeryLongInt::toUint32 - number too big");
  }

  if (isZero())
    return 0;
  else
    return (uint32_t) at(0);
}

std::pair<VeryLongInt, VeryLongInt> VeryLongInt::divide_modulo_impl(
    const VeryLongInt& numerator,
    const VeryLongInt& denominator) {

  VeryLongInt quotient = 0;
  VeryLongInt modulo = numerator;

  VeryLongInt n = denominator;
  VeryLongInt power = 1;

  if (n.numberOfBinaryDigits() < modulo.numberOfBinaryDigits()) {
    size_t shift = (size_t)(modulo.numberOfBinaryDigits() - n.numberOfBinaryDigits());
    n <<= shift;
    power <<= shift;
  }

  while (n <= modulo) {
    n <<= 1;
    power <<= 1;
  }

  while (!power.isZero()) {
    assert(!n.isZero());
    if (n <= modulo) {
      modulo -= n;
      quotient += power;
    }
    n >>= 1;
    power >>= 1;
  }

  assert(modulo < denominator);
  return std::make_pair(quotient, modulo);
}

void VeryLongInt::init_from_string(const std::string& number) {
  data.reset(new container_type());
  if (number.empty() || (number[0] == '0' && number.size() > 1)) {
    setNaN();
    return;
  }

  for (char c : number) {
    if (!isdigit(c)) {
      setNaN();
      return;
    }

    operator*=(10);
    unsigned int digit = (unsigned int) (c - '0');
    operator+=(digit);
  }
}

void VeryLongInt::normalize() {
  if (isValid()) {
    while (!data->empty() && data->back() == 0)
      data->pop_back();
  }
}

void VeryLongInt::update_carry() {
  value_type carry = 0;
  for (size_t i = 0; i < length(); i++) {
    at(i) += carry;
    carry = at(i) / base;
    at(i) %= base;
  }
  assert(carry == 0);
}

std::ostream& operator<<(std::ostream& stream, const VeryLongInt& number) {
  if (!number.isValid()) {
    stream << "NaN";
  }
  else if (number.isZero()) {
    stream << "0";
  }
  else {
    VeryLongInt n = number;
    std::string result;
    while (!n.isZero()) {
      uint32_t digit = (n % 10).toUint32();
      result.push_back('0' + (char) digit);
      n /= 10;
    }

    std::reverse(result.begin(), result.end());
    stream << result;
  }
  return stream;
}

const VeryLongInt& Zero() {
  static VeryLongInt zero = 0;
  return zero;
}

const VeryLongInt& NaN() {
  static VeryLongInt result;
  static bool initialized = false;
  if (!initialized) {
    result.setNaN();
    initialized = true;
  }
  return result;
}
