#include <string>
#include <iostream>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;

typedef unsigned long long currency_value_type;

const string kWhitespacesPattern = "\\s+";
const string kOptionalWhitespacesPattern = "\\s*";
const string kCurrencyValuePattern = "\\b((?:[1-9][0-9]*)|0)(?:,([0-9]{1,3}))?\\b";
const string kCurrencyISOCodePattern = "([A-Z]{3})";
const string kDonorNamePattern = "((?:\\S)|(?:\\S.*\\S))";
const string kCurrencyExchangeRatePattern =
    kOptionalWhitespacesPattern +
    kCurrencyISOCodePattern +
    kWhitespacesPattern +
    kCurrencyValuePattern +
    kOptionalWhitespacesPattern;
const string kDonorLinePattern =
    kOptionalWhitespacesPattern +
    kDonorNamePattern +
    kWhitespacesPattern +
    kCurrencyValuePattern +
    kWhitespacesPattern +
    kCurrencyISOCodePattern +
    kOptionalWhitespacesPattern;
const string kQueryPattern =
    kOptionalWhitespacesPattern +
    kCurrencyValuePattern +
    kWhitespacesPattern +
    kCurrencyValuePattern +
    kOptionalWhitespacesPattern;

const currency_value_type kCurrencyMultiplyFactor = 1000;

enum class ReadingState {
  kCurrencyExchangeValues,
  kDonors,
  kQueries
};

typedef tuple<string, currency_value_type> currency_exchange_type;
typedef tuple<string, currency_value_type, string> donor_type;
typedef tuple<currency_value_type, currency_value_type> query_type;
typedef unordered_map<string, currency_value_type> exchange_rate_map_type;

bool MatchRegex(const string& sequence, boost::smatch& match_data, const string& pattern) {
  boost::regex compiledPattern(pattern);
  return boost::regex_match(sequence, match_data, compiledPattern);
}

currency_value_type ToCurrencyValue(const string& beforeComma, string afterComma) {
  while (afterComma.size() < 3)
    afterComma += '0';

  stringstream stream(beforeComma + afterComma);
  currency_value_type result;
  stream >> result;
  if (stream.fail())
    throw invalid_argument("ToCurrencyValue: invalid argument");
  return result;
}

bool TryParseCurrencyExchangeRate(const string &line, currency_exchange_type& result) {
  boost::smatch matches;
  bool matched = MatchRegex(line, matches, kCurrencyExchangeRatePattern);

  if (!matched)
    return false;

  try {
    get<0>(result) = matches[1];
    get<1>(result) = ToCurrencyValue(matches[2], matches[3]);
    return true;
  }
  catch (const invalid_argument& e) {
    return false;
  }
}

bool TryParseDonor(const string &line, donor_type& result) {
  boost::smatch matches;
  bool matched = MatchRegex(line, matches, kDonorLinePattern);

  if (!matched)
    return false;

  try {
    get<0>(result) = matches[1];
    get<1>(result) = ToCurrencyValue(matches[2], matches[3]);
    get<2>(result) = matches[4];
    return true;
  }
  catch (const invalid_argument& e) {
    return false;
  }
}

bool TryParseQuery(const string &line, query_type& result) {
  boost::smatch matches;
  bool matched = MatchRegex(line, matches, kQueryPattern);

  if (!matched)
    return false;

  try {
    get<0>(result) = ToCurrencyValue(matches[1], matches[2]);
    get<1>(result) = ToCurrencyValue(matches[3], matches[4]);
    return true;
  }
  catch (const invalid_argument& e) {
    return false;
  }
}

template<class T1, class T2>
ostream& operator<<(ostream& stream, const tuple<T1, T2>& t) {
  stream << "(" << get<0>(t) << ", " << get<1>(t) << ")";
  return stream;
}

template<class T1, class T2, class T3>
ostream& operator<<(ostream& stream, const tuple<T1, T2, T3>& t) {
  stream << "(" << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << ")";
  return stream;
}

currency_value_type BankersRounding(currency_value_type value, currency_value_type power) {
  currency_value_type result = value / (power / 10);
  currency_value_type lastDigit = result % 10;
  result /= 10;
  currency_value_type penultimateDigit = result % 10;
  return (lastDigit == 5 && penultimateDigit % 2) || lastDigit > 5 ? result + 1 : result;
}

currency_value_type StableMultiply(currency_value_type a, currency_value_type b, currency_value_type power) {
  currency_value_type tail1 = a % power;
  currency_value_type tail2 = b % power;
  currency_value_type head1 = a / power;
  currency_value_type head2 = b / power;
  currency_value_type rest = BankersRounding((tail1 * head2 +
                                             tail2 * head1) * power +
                                             tail1 * tail2, power);
  return head1 * head2 * power + rest;
}

currency_value_type CalculateCurrencyValue(const donor_type& donor,
                                           const exchange_rate_map_type&
                                           currency_exchange_rates) {
  currency_value_type exchange_value = currency_exchange_rates.find(get<2>(donor))->second;
  return StableMultiply(exchange_value,
                        get<1>(donor),
                        kCurrencyMultiplyFactor);
}

template<class Iterator>
pair<Iterator, Iterator> CalculateQueryRange(Iterator begin,
                                             Iterator end,
                                             const query_type& query,
                                             const exchange_rate_map_type &currency_exchange_rates) {
  Iterator result_begin = lower_bound(begin,
                           end,
                           get<0>(query),
                           [&currency_exchange_rates](const donor_type &a, currency_value_type value) {
                             return CalculateCurrencyValue(a, currency_exchange_rates) < value;
                           });

  Iterator result_end = upper_bound(begin,
                         end,
                         get<1>(query),
                         [&currency_exchange_rates](currency_value_type value, const donor_type &b) {
                           return value < CalculateCurrencyValue(b, currency_exchange_rates);
                         });

  return make_pair(result_begin, result_end);
};

bool IsCurrencyValueInRange(currency_value_type value) {
    /* value <= (kCurrencyMultiplyFactor * Max_long + kCurrencyMultiplyFactor / 2 - 1) /
                (kCurrencyMultiplyFactor * Max_int + kCurrencyMultiplyFactor - 1) */
    currency_value_type temp = numeric_limits<unsigned int>::max();
    return value <=
        (numeric_limits<unsigned long long>::max()) /
        (temp + 1) + 1 && value > 0;
}

bool IsDonorValueInRange(donor_type donor) {
    currency_value_type temp = numeric_limits<unsigned int>::max();
    currency_value_type value = get<1>(donor);
    return value <= (temp + 1) * kCurrencyMultiplyFactor - 1 && value > 0;
}

bool IsDefinedCurrency(const string &currency_name,
                       const exchange_rate_map_type &currency_exchange_rates) {
    return currency_exchange_rates.find(currency_name) != currency_exchange_rates.end();
}

int main() {
  ios::sync_with_stdio(false);

  vector<donor_type> donors;
  unordered_map<string, currency_value_type> currency_exchange_rates;
  ReadingState reading_state = ReadingState::kCurrencyExchangeValues;

  string line;
  cout << boolalpha;
  size_t lineNumber = 1;

  auto donorsComparator = [&currency_exchange_rates](donor_type a, donor_type b){
    return CalculateCurrencyValue(a, currency_exchange_rates) <
           CalculateCurrencyValue(b, currency_exchange_rates);
  };

  while (getline(cin, line)) {
    currency_exchange_type currency_exchange;
    donor_type donor;
    query_type query;

    bool valid_line = false;
    bool sorted = false;

    if (TryParseCurrencyExchangeRate(line, currency_exchange)) {
      //cout << "CurrencyExchangeRate: " << currency_exchange << endl;

      bool valid_state = reading_state == ReadingState::kCurrencyExchangeValues;
      bool valid_input = IsCurrencyValueInRange(get<1>(currency_exchange)) &&
                        !IsDefinedCurrency(get<0>(currency_exchange), currency_exchange_rates);
      if (valid_state && valid_input) {
        valid_line = true;

        currency_exchange_rates.insert(make_pair(get<0>(currency_exchange), get<1>(currency_exchange)));
      }
    }
    else if (TryParseDonor(line, donor)) {
      //cout << "Donor: " << donor << endl;

      bool valid_state = reading_state == ReadingState::kDonors ||
                         reading_state == ReadingState::kCurrencyExchangeValues;
      bool valid_input = IsDefinedCurrency(get<2>(donor), currency_exchange_rates) &&
                         IsDonorValueInRange(donor);
      if (valid_state && valid_input) {
        valid_line = true;
        reading_state = ReadingState::kDonors;

        donors.push_back(donor);
      }
    }
    else if (TryParseQuery(line, query)) {
      //cout << "Query: " << query << endl;

      if(!sorted) {
        stable_sort(donors.begin(), donors.end(), donorsComparator);
        sorted = true;
      }

      bool valid_input = get<0>(query) <= get<1>(query);
      if (valid_input) {
        valid_line = true;
        reading_state = ReadingState::kQueries;

        valid_line = get<0>(query) <= get<1>(query);

        auto range = CalculateQueryRange(donors.begin(), donors.end(), query,
                                         currency_exchange_rates);
        for (auto it = range.first; it != range.second; ++it) {
          const string& name = get<0>(*it);
          currency_value_type value = get<1>(*it);
          const string& currency = get<2>(*it);
          cout << "\"" << name << "\",\"";
          cout << value / 1000  << "," << setfill('0') << setw (3) << value % 1000;
          cout << "\"," << currency << endl;
        }
      }
    }

    if (!valid_line) {
      cerr << "Error in line " << lineNumber << ":" << line << endl;
    }

    lineNumber++;
  }
}
