#ifndef VERY_LONG_INT_H
#define VERY_LONG_INT_H

#include <ostream>
#include <stdint.h>
#include <vector>
#include <memory>

class VeryLongInt {
 public:
  typedef uint64_t value_type;
  typedef std::vector<value_type> container_type;

  static constexpr value_type base_digits_number = 32;
  static constexpr value_type base = 1uLL << base_digits_number;

  VeryLongInt();

  VeryLongInt(const VeryLongInt&);

  VeryLongInt(VeryLongInt&&);

  template<typename Integer, typename =
  typename std::enable_if<std::is_integral<Integer>::value, Integer>::type>
  VeryLongInt(Integer n) {
    static_assert(std::is_integral<Integer>::value, "Integer required.");
    data.reset(new container_type());
    while (n > 0) {
      data->push_back(n % base);
      n /= base;
    }
  }

  VeryLongInt(bool val) = delete;

  VeryLongInt(char val) = delete;

  explicit VeryLongInt(const std::string& number);

  explicit VeryLongInt(const char * number);

  VeryLongInt& operator=(const VeryLongInt&);

  VeryLongInt& operator=(VeryLongInt&&);

  template<typename Integer, typename =
  typename std::enable_if<std::is_integral<Integer>::value, Integer>::type>
  VeryLongInt& operator=(Integer n) {
    static_assert(std::is_integral<Integer>::value, "Integer required.");
    *this = std::move(VeryLongInt(n));
    return *this;
  }

  VeryLongInt& operator=(bool val) = delete;

  VeryLongInt& operator=(char val) = delete;

  int numberOfBinaryDigits() const;

  bool isValid() const;

  bool isZero() const;

  VeryLongInt& operator+=(const VeryLongInt&);

  VeryLongInt& operator-=(const VeryLongInt&);

  VeryLongInt& operator*=(const VeryLongInt&);

  VeryLongInt& operator/=(const VeryLongInt&);

  VeryLongInt& operator%=(const VeryLongInt&);

  VeryLongInt& operator<<=(unsigned int);

  VeryLongInt& operator>>=(unsigned int);

  friend const VeryLongInt operator+(const VeryLongInt&, const VeryLongInt&);

  friend const VeryLongInt operator-(const VeryLongInt&, const VeryLongInt&);

  friend const VeryLongInt operator*(const VeryLongInt&, const VeryLongInt&);

  friend const VeryLongInt operator/(const VeryLongInt&, const VeryLongInt&);

  friend const VeryLongInt operator%(const VeryLongInt&, const VeryLongInt&);

  const VeryLongInt operator<<(unsigned int) const;

  const VeryLongInt operator>>(unsigned int) const;

  friend bool operator==(const VeryLongInt&, const VeryLongInt&);

  friend bool operator!=(const VeryLongInt&, const VeryLongInt&);

  friend bool operator<(const VeryLongInt&, const VeryLongInt&);

  friend bool operator<=(const VeryLongInt&, const VeryLongInt&);

  friend bool operator>(const VeryLongInt&, const VeryLongInt&);

  friend bool operator>=(const VeryLongInt&, const VeryLongInt&);

  friend const VeryLongInt& NaN();

  explicit operator bool() const;

  uint32_t toUint32() const;

 private:
  static std::pair<VeryLongInt, VeryLongInt> divide_modulo_impl(
      const VeryLongInt& numerator,
      const VeryLongInt& denominator);

  void init_from_string(const std::string& number);

  value_type& at(size_t index) {
    return data->at(index);
  }

  value_type at(size_t index) const {
    return data->at(index);
  }

  size_t length() const {
    return data->size();
  }

  void setNaN() {
    data.reset();
  }

  // delete leading zeros
  void normalize();

  void update_carry();

  void resize(size_t new_length) {
    if (isValid()) data->resize(new_length);
  }

  std::unique_ptr<container_type> data;
};

std::ostream& operator<<(std::ostream&, const VeryLongInt&);

const VeryLongInt& Zero();

const VeryLongInt& NaN();

#endif // VERY_LONG_INT_H