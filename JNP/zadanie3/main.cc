#include <iostream>
#include <sstream>
#include <assert.h>
#include "very_long_int.h"

using namespace std;

#define assert_equal(x, y) if ((x) != (y)) cerr << x << " != " << y << "!" << endl; assert(x == y);

void TestyZTresci();
void MojeTesty();

int main() {
  MojeTesty();
  TestyZTresci();
}


void TestyZTresci() {
  {
    VeryLongInt x = 1;
    x /= 0;
    assert(!x.isValid());
  }

  {
    VeryLongInt x = 100;
    x -= 101;
    assert(!x.isValid());
  }

  {
    VeryLongInt x = 23;
    VeryLongInt y = x;
    assert(x == y);
  }

  {
    VeryLongInt x = 23;
    VeryLongInt y = 32;
    x = y;
    assert(x == y);
  }

  {
    VeryLongInt x = 23;
    VeryLongInt y = 32;
    assert(y > x);
  }

  {
    VeryLongInt x = 23;
    VeryLongInt y = 32;
    assert(y >= x);
  }

  {
    VeryLongInt x = NaN();
    assert(!x.isValid());
  }

  {
    VeryLongInt x = 10;
    if (x)
      assert(1);
    else
      assert(0);
  }

  {
    VeryLongInt x = 1;
    x <<= 123;
    x >>= 120;
    assert(8 == x);
  }

  {
    VeryLongInt x = 1;
    for (int i = 1; i <= 100; ++i)
      x *= 2;
    assert(x % 3 == 1);
  }

  {
    VeryLongInt x = Zero();
    assert(x == 0);
  }

  {
    const int N = 100;
    VeryLongInt x = 1;
    for (int i = 1; i < N; ++i)
      x *= 2;
    assert(x.numberOfBinaryDigits() == N);
  }

  {
    VeryLongInt x("1234567890123456789012345678901234567890");
    VeryLongInt z = x;
    VeryLongInt y("777777777777777777777777777777777777777");
    x = x + y;
    x -= y;
    assert(x == z);
  }

  {
    VeryLongInt x(string("12345678"));
    VeryLongInt y(12345678U);
    assert(x == y);
  }

  {
    VeryLongInt x("12345678901234567890");
    VeryLongInt y(12345678901234567890UL);
    assert(x == y);
    cout << y << endl;
  }

  {
    VeryLongInt x("1234567890123456789012345678901234567890");
    VeryLongInt y("1204567890123456789012345678901234567890");
    VeryLongInt z(  "30000000000000000000000000000000000000");
    assert(z == x - y);
  }

  {
    VeryLongInt x("10000000000");
    VeryLongInt y("100000000000");
    VeryLongInt z("1000000000000000000000");
    assert(z == x * y);
  }

  {
    const int N = 100;
    VeryLongInt x = 1;
    for (int i = 2; i <= N; ++i)

      x *= i;
    for (int i = 2; i <= N; ++i)
      x /= i;
    assert(x == 1);
  }

  {
    assert(Zero().numberOfBinaryDigits() == 1);
    assert(NaN().numberOfBinaryDigits() == 0);
  }
}

#define COMPILATION_TEST 0

void NotCompiling() {

#if COMPILATION_TEST == 1
  VeryLongInt a('1'); // b³¹d kompilacji
#endif

#if COMPILATION_TEST == 2
  VeryLongInt b(true); // b³¹d kompilacji
#endif

#if COMPILATION_TEST == 3
  VeryLongInt c;
  c += "123"; // b³¹d kompilacji
#endif

#if COMPILATION_TEST == 4
  VeryLongInt d;
  d -= ::std::string("123"); // b³¹d kompilacji
#endif

#if COMPILATION_TEST == 5
  VeryLongInt e, f, g;
  e * g = h; // b³¹d kompilacji
#endif


#if COMPILATION_TEST == 6
  VeryLongInt h;
  int i = h; // b³¹d kompilacji
#endif
}

// to powinno siê kompilowaæ
void Comipiling() {
  VeryLongInt a = (int)1;
  VeryLongInt b = (long)1;
  VeryLongInt c = (long long)1;
  VeryLongInt d = (short)1;
  VeryLongInt e = (short int)1;

  VeryLongInt au = (unsigned int)1;
  VeryLongInt bu = (unsigned long)1;
  VeryLongInt cu = (unsigned long long)1;
  VeryLongInt du = (unsigned short)1;
  VeryLongInt eu = (unsigned short int)1;
}

void MojeTesty() {
  {
    VeryLongInt a;
    VeryLongInt b = 0;
    VeryLongInt c = 1;
    assert(a.isZero());
    assert(b.isZero());
    assert(!c.isZero());
  }

  {
    VeryLongInt a = 1, b = 1;
    assert_equal(a * b, 1);
  }

  {
    VeryLongInt a = 2, b = 3;
    assert_equal(a * b, 6);
  }

  {
    VeryLongInt a = 1024 * 1024 * 1024;
    assert_equal(a * a, 1024uLL * 1024 * 1024 * 1024 * 1024 * 1024);
  }

  {
    VeryLongInt a = 124;
    assert_equal((a << 1), 248);
    assert_equal((a >> 1), 62);
    assert_equal((a >> 2), 31);
    assert_equal((a >> 3), 15);

    VeryLongInt b = 1024uLL * 1024 * 1024 * 1024;
    assert_equal((b >> 20), 1024 * 1024);
  }

  {
    stringstream stream;
    VeryLongInt a("1234567890123456789012345678901234567890");
    stream << a;
    assert(stream.str() == "1234567890123456789012345678901234567890");

    VeryLongInt b = 0;
    stream.str("");
    stream << b;
    assert(stream.str() == "0");

    VeryLongInt c = NaN();
    stream.str("");
    stream << c;
    assert(stream.str() == "NaN");
  }



  {
    VeryLongInt a;
    assert(a.isValid());
    assert(a.isZero());
  }

  {
    VeryLongInt a = 123;
    VeryLongInt b = a;
    assert(b == 123);
    assert(a.isValid());
  }

  {
    VeryLongInt a = 123;
    VeryLongInt b = std::move(a);
    assert(b == 123);
    assert(!a.isValid());
  }

  {
    VeryLongInt a = 123LL;
    assert(a == 123);
  }

  {
    VeryLongInt a("123");
    assert(a == 123);
  }

  {
    VeryLongInt a(std::string("123"));
    assert(a == 123);
  }

  {
    VeryLongInt a = 123;
    VeryLongInt b;
    b = a;
    assert(b == 123);
    assert(a == 123);
  }

  {
    VeryLongInt a = 123;
    VeryLongInt b;
    b = std::move(a);
    assert(b == 123);
    assert(!a.isValid());
  }

  {
    VeryLongInt a = 123;
    a = 456;
    assert(a == 456);
  }

  {
    VeryLongInt a("18889465931478580856019");
    assert(a.numberOfBinaryDigits() == 75);
    assert(Zero().numberOfBinaryDigits() == 1);
    assert(NaN().numberOfBinaryDigits() == 0);
    assert((a << 1000).numberOfBinaryDigits() == 1075);
  }

  {
    VeryLongInt a;
    VeryLongInt b = 0;
    VeryLongInt c("0");
    VeryLongInt d = Zero();
    assert(a.isValid());
    assert(b.isValid());
    assert(c.isValid());
    assert(d.isValid());
    assert(a.isZero());
    assert(b.isZero());
    assert(c.isZero());
    assert(d.isZero());
  }

  {
    VeryLongInt a = NaN();
    assert(!a.isValid());
    a += 3;
    assert(!a.isValid());

    VeryLongInt b = 0;
    b += NaN();
    assert(!b.isValid());
  }

  {
    VeryLongInt a = NaN();
    assert(!a.isValid());
    a -= 3;
    assert(!a.isValid());

    VeryLongInt b = 100;
    b -= NaN();
    assert(!b.isValid());
  }

  {
    VeryLongInt a = NaN();
    assert(!a.isValid());
    a *= 3;
    assert(!a.isValid());

    VeryLongInt b = 100;
    b *= NaN();
    assert(!b.isValid());
  }

  {
    VeryLongInt a = NaN();
    assert(!a.isValid());
    a /= 3;
    assert(!a.isValid());

    VeryLongInt b = 100;
    b /= NaN();
    assert(!b.isValid());
  }

  {
    VeryLongInt a = NaN();
    assert(!a.isValid());
    a %= 3;
    assert(!a.isValid());

    VeryLongInt b = 100;
    b %= NaN();
    assert(!b.isValid());
  }

  {
    VeryLongInt a = NaN();
    assert(!a.isValid());
    a <<= 3;
    assert(!a.isValid());
  }

  {
    VeryLongInt a = NaN();
    assert(!a.isValid());
    a >>= 3;
    assert(!a.isValid());

    VeryLongInt b = 100;
    b >>= 10000;
    assert(b.isValid());
    assert(b.isZero());
  }

  {
    assert(!(NaN() + 1).isValid());
    assert(!(1 + NaN()).isValid());
    assert(!(NaN() + NaN()).isValid());
  }

  {
    assert(!(NaN() - 1).isValid());
    assert(!(1 - NaN()).isValid());
    assert(!(NaN() - NaN()).isValid());
  }

  {
    assert(!(NaN() * 1).isValid());
    assert(!(1 * NaN()).isValid());
    assert(!(NaN() * NaN()).isValid());

    assert((Zero() * 1).isZero());
    assert((1 * Zero()).isZero());
    assert((Zero() * Zero()).isZero());


    assert(!(Zero() * NaN()).isValid());
    assert(!(NaN() * Zero()).isValid());
  }

  {
    assert(!(NaN() / 1).isValid());
    assert(!(1 / NaN()).isValid());
    assert(!(NaN() / NaN()).isValid());

    assert((Zero() / 1).isZero());
    assert(!(1 / Zero()).isValid());
    assert(!(Zero() / Zero()).isValid());


    assert(!(Zero() / NaN()).isValid());
    assert(!(NaN() / Zero()).isValid());
  }

  {
    assert(!(NaN() % 1).isValid());
    assert(!(1 % NaN()).isValid());
    assert(!(NaN() % NaN()).isValid());

    assert((Zero() % 1).isZero());
    assert(!(1 % Zero()).isValid());
    assert(!(Zero() % Zero()).isValid());


    assert(!(Zero() % NaN()).isValid());
    assert(!(NaN() % Zero()).isValid());
  }

  {
    assert( !(NaN() >> 1).isValid());
  }

  {
    assert( !(NaN() << 1).isValid());
  }

  {
    assert(Zero() == Zero());
    assert(!(NaN() == NaN()));
    assert(!(NaN() == Zero()));
    assert(!(Zero() == NaN()));

    assert(Zero() == 0);
    assert(0 == Zero());
    assert(!(NaN() == 0));
    assert(!(0 == NaN()));
  }

  {
    assert(!(Zero() != Zero()));
    assert(!(NaN() != NaN()));
    assert(!(NaN() != Zero()));
    assert(!(Zero() != NaN()));

    assert(!(Zero() != 0));
    assert(!(0 != Zero()));
    assert(!(NaN() != 0));
    assert(!(0 != NaN()));
  }

  {
    assert(!(Zero() < Zero()));
    assert(!(NaN() < NaN()));
    assert(!(NaN() < Zero()));
    assert(!(Zero() < NaN()));

    assert(!(Zero() < 0));
    assert(!(0 < Zero()));
    assert(!(NaN() < 0));
    assert(!(0 < NaN()));

    assert(Zero() < 1);
    assert(!(1 < Zero()));
  }

  {
    assert(Zero() <= Zero());
    assert(!(NaN() <= NaN()));
    assert(!(NaN() <= Zero()));
    assert(!(Zero() <= NaN()));

    assert(Zero() <= 0);
    assert(0 <= Zero());
    assert(!(NaN() <= 0));
    assert(!(0 <= NaN()));

    assert(Zero() <= 1);
    assert(!(1 <= Zero()));
  }

  {
    assert(!(Zero() > Zero()));
    assert(!(NaN() > NaN()));
    assert(!(NaN() > Zero()));
    assert(!(Zero() > NaN()));

    assert(!(Zero() > 0));
    assert(!(0 > Zero()));
    assert(!(NaN() > 0));
    assert(!(0 > NaN()));

    assert(!(Zero() > 1));
    assert(1 > Zero());
  }

  {
    assert(Zero() >= Zero());
    assert(!(NaN() >= NaN()));
    assert(!(NaN() >= Zero()));
    assert(!(Zero() >= NaN()));

    assert(Zero() >= 0);
    assert(0 >= Zero());
    assert(!(NaN() >= 0));
    assert(!(0 >= NaN()));

    assert(!(Zero() >= 1));
    assert(1 >= Zero());
  }

  {
    assert(!NaN().isValid());
  }

  {
    bool a(NaN());
    bool b(VeryLongInt(0));
    bool c(VeryLongInt(1));
    bool d(VeryLongInt(2));
    bool e(Zero());

    assert(!a);
    assert(!b);
    assert(c);
    assert(d);
    assert(!e);
  }
}