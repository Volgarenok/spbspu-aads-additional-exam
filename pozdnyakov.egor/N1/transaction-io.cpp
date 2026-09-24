#include "transaction-io.hpp"
#include <cctype>
#include <cstddef>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include "array.hpp"
#include "transaction.hpp"

namespace {
  bool isSpace(char symbol)
  {
    return std::isspace(static_cast< unsigned char >(symbol)) != 0;
  }

  bool isDigit(char symbol)
  {
    return std::isdigit(static_cast< unsigned char >(symbol)) != 0;
  }

  void skipSpaces(const std::string & line, std::size_t & position)
  {
    while (position < line.size() && isSpace(line[position])) {
      ++position;
    }
  }

  bool parseInteger(const std::string & line, std::size_t & position, int & result)
  {
    constexpr long long base = 10;
    constexpr long long max_value = std::numeric_limits< int >::max();
    constexpr long long min_value = std::numeric_limits< int >::min();
    skipSpaces(line, position);
    bool negative = false;
    if (position < line.size() && (line[position] == '-' || line[position] == '+')) {
      negative = line[position] == '-';
      ++position;
    }
    if (position == line.size() || !isDigit(line[position])) {
      return false;
    }
    long long value = 0;
    while (position < line.size() && isDigit(line[position])) {
      value = value * base + (line[position] - '0');
      if (value > -min_value) {
        return false;
      }
      ++position;
    }
    if (position < line.size() && !isSpace(line[position])) {
      return false;
    }
    value = negative ? -value : value;
    if (value > max_value) {
      return false;
    }
    result = static_cast< int >(value);
    return true;
  }

  bool parseAccount(const std::string & line, std::size_t & position, int & account)
  {
    int number = 0;
    if (!parseInteger(line, position, number) || number == std::numeric_limits< int >::min()) {
      return false;
    }
    account = number < 0 ? -number : number;
    return true;
  }

  bool isBlank(const std::string & line)
  {
    std::size_t position = 0;
    skipSpaces(line, position);
    return position == line.size();
  }
}

bool pozdnyakov::parseTransaction(const std::string & line, Transaction & transaction)
{
  std::size_t position = 0;
  Transaction result{0, 0, 0};
  if (!parseAccount(line, position, result.from)) {
    return false;
  }
  if (!parseAccount(line, position, result.to)) {
    return false;
  }
  if (!parseInteger(line, position, result.value)) {
    return false;
  }
  skipSpaces(line, position);
  if (position != line.size()) {
    return false;
  }
  transaction = result;
  return true;
}

pozdnyakov::ReadStatistics pozdnyakov::readTransactions(std::istream & in, Array< Transaction > & transactions)
{
  ReadStatistics statistics{0, 0};
  std::string line;
  while (std::getline(in, line)) {
    if (isBlank(line)) {
      continue;
    }
    Transaction transaction{0, 0, 0};
    if (parseTransaction(line, transaction) && transaction.from != transaction.to) {
      pushBack(transactions, transaction);
      ++statistics.accepted;
    } else {
      ++statistics.ignored;
    }
  }
  return statistics;
}

std::ostream & pozdnyakov::operator<<(std::ostream & out, const Transaction & transaction)
{
  return out << transaction.from << ' ' << transaction.to << ' ' << transaction.value;
}

void pozdnyakov::writeTransactions(std::ostream & out, const Array< Transaction > & transactions)
{
  for (std::size_t i = 0; i < transactions.size; ++i) {
    out << transactions.data[i] << '\n';
  }
}
