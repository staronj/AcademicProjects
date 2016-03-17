#include "very_long_int.h"
#include <iostream>
#include <climits>
#include <cassert>
#include <sstream>
#include <random>

using namespace std;

int main() {
  {
    VeryLongInt p = 3;
    VeryLongInt z = 4;
    p = 42;
    stringstream s;
    s << p << " " << z << " " << NaN();
    assert(s.str() == string("42 4 NaN"));
  }
  {
    VeryLongInt zero = 0;
    zero -= 0;
    assert (zero == 0);
  }

  {
    VeryLongInt a = 2;
    a /= 2;
    assert(a == 1);
  }
  {
    VeryLongInt a = 42;
    VeryLongInt b(a);
    assert(42 == b);
  }

  {
    VeryLongInt a = 42;
    assert(a <= 42);
    assert(a < 43);
    assert(a > 41);
    assert(a >= 42);
    assert(a == 42);
    assert(a != 43);
    assert(42 >= a);
    assert(43 > a);
    assert(41 < a);
    assert(42 <= a);
    assert(41 <= a);
    assert(42 == a);
    assert(44 != a);
    VeryLongInt p = 3;
    assert(p <= a);
    assert(p < a);
    assert(a > p);
    assert(a >= p);
    assert(a == a);

  }


  {
    VeryLongInt x = 1;
    x /= 0;
    assert(!x.isValid());
  }
  {
    VeryLongInt x = 42;
    x /= NaN();
    assert(!x.isValid());
    assert(x.numberOfBinaryDigits() == 0);
    VeryLongInt x2 = x;
    assert(!x2.isValid());
    assert(x2.numberOfBinaryDigits() == 0);
    assert(!(x != NaN()));
    assert(!(NaN() != x));
    assert(!(x2 != x));
    assert(!(x2 == NaN()));
    assert(!(x2 == NaN()));
    assert(!(x2 == NaN()));
    assert(!(x2 == 5));
    assert(!(42 != x));
    assert(!(42 < x));
    assert(!(x < 42));
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
      x <<= 1;
    assert(x.numberOfBinaryDigits() == N);
  }
  {
    const int N = 100;
    VeryLongInt x = 1;
    for (int i = 1; i < N; ++i)
      x *= 2;
    //assert(x.numberOfBinaryDigits() == N);
  }

  {  //Jedyny (na razie) test który nie działa
    VeryLongInt x("1234567890123456789012345678901234567890");
    stringstream s;
    s << x;
    assert(s.str() == string("1234567890123456789012345678901234567890"));

    VeryLongInt z = x;
    VeryLongInt y("777777777777777777777777777777777777777");
    x = x + y;
    x -= y;
    assert(x == z);
    stringstream s2;
    s2 << y;
    assert(s2.str() == string("777777777777777777777777777777777777777"));
  }

  {
    VeryLongInt x("12345");
    VeryLongInt y(12345);
    assert(x == y);
  }

  {
    VeryLongInt x("12345678901234567890");
    VeryLongInt y(12345678901234567890UL);
    assert(x == y);
  }

  {
    VeryLongInt x("1234567890123456789012345678901234567890");
    VeryLongInt y("1204567890123456789012345678901234567890");
    VeryLongInt z("30000000000000000000000000000000000000");
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

  {
    random_device d;
    mt19937 gen(d());
    for (int i = 0; i < 500000; i++) {
      long long a = gen() % 10000;
      long long b = gen() % 10000;
      VeryLongInt x(a), y(b);
      if (x + y != a + b) {
        cout << "Błąd w dodawaniu:" << endl;
        cout << a << " " << b << endl;

      }
      {
        stringstream s, s2;
        s << x + y;
        s2 << a + b;
        if (s.str() != s2.str()) {
          cout << "Dodawanie fail" << endl;
          cout << a << " " << b << endl;
          cout << s.str() << " != " << s2.str() << endl;
        }
      }
      if (a - b < 0) {
        if ((x - y).isValid()) {
          cout << "Błąd w odejmowaniu:" << endl;
          cout << a << " " << b << endl;
        }
      }
      else {
        if (x - y != a - b) {
          cout << "Błąd w odejmowaniu:" << endl;
          cout << a << " " << b << endl;

        }

        stringstream s, s2;
        s << x - y;
        s2 << a - b;
        if (s.str() != s2.str()) {
          cout << "Odejmowanie fail" << endl;
          cout << a << " " << b << endl;
          cout << s.str() << " != " << s2.str() << endl;
        }

      }
      {
        stringstream s, s2;
        s << x * y;
        s2 << a * b;
        if (s.str() != s2.str()) {
          cout << "Mnożenie fail" << endl;
          cout << a << " " << b << endl;
          cout << s.str() << " != " << s2.str() << endl;
        }
      }
      if (x * y != a * b) {
        cout << "Błąd w mnożeniu:" << endl;
        cout << a << " " << b << endl;
      }

      if (b == 0) {
        if ((x / y).isValid()) {
          cout << "Błąd w dzieleniu div:" << endl;
          cout << a << " " << b << endl;
        }
        if ((x % y).isValid()) {
          cout << "Błąd w dzieleniu mod:" << endl;
          cout << a << " " << b << endl;
        }
      }
      else {
        if (x / y != a / b) {
          cout << "Błąd w dzieleniu div:" << endl;
          cout << a << " " << b << endl;
        }

        if (x % y != a % b) {
          cout << "Błąd w dzieleniu mod:" << endl;
          cout << a << " " << b << endl;
        }

        {
          stringstream s, s2;
          s << x % y;
          s2 << a % b;
          if (s.str() != s2.str()) {
            cout << "modulo fail" << endl;
            cout << a << " " << b << endl;
            cout << s.str() << " != " << s2.str() << endl;
          }
        }
        {
          stringstream s, s2;
          s << x / y;
          s2 << a / b;
          if (s.str() != s2.str()) {
            cout << "Dzielenie fail" << endl;
            cout << a << " " << b << endl;
            cout << s.str() << " != " << s2.str() << endl;
          }
        }

      }

      if ( (i + 1) % 1000 == 0) {
        cout << "Zrobiono " << (i + 1) << " testow." << endl;
      }
    }
  }
}
