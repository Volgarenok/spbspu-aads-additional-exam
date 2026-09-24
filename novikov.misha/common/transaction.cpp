#include "transaction.h"

#include <cctype>
#include <cstdlib>

namespace novikov {

namespace {

bool parseSignedInt(const std::string &line, std::size_t &pos, int &value)
{
  const std::size_t length = line.size();
  while (pos < length && std::isspace(static_cast<unsigned char>(line[pos])) != 0) {
    ++pos;
  }

  bool negative = false;
  if (pos < length && (line[pos] == '-' || line[pos] == '+')) {
    negative = (line[pos] == '-');
    ++pos;
  }

  if (pos >= length || std::isdigit(static_cast<unsigned char>(line[pos])) == 0) {
    return false;
  }

  int result = 0;
  while (pos < length && std::isdigit(static_cast<unsigned char>(line[pos])) != 0) {
    result = result * 10 + (line[pos] - '0');
    ++pos;
  }

  value = negative ? -result : result;
  return true;
}

}

bool parseTransactionLine(const std::string &line, Transaction &result)
{
  std::size_t pos = 0;
  int from = 0;
  int to = 0;
  int value = 0;

  if (!parseSignedInt(line, pos, from) || !parseSignedInt(line, pos, to) || !parseSignedInt(line, pos, value)) {
    return false;
  }

  while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos])) != 0) {
    ++pos;
  }
  if (pos != line.size()) {
    return false;
  }

  result.from = std::abs(from);
  result.to = std::abs(to);
  result.value = value;
  return true;
}

void readTransactions(std::istream &input, DynamicArray<Transaction> &transactions, std::size_t &ignoredCount)
{
  std::string line;
  while (std::getline(input, line)) {
    Transaction transaction{0, 0, 0};
    if (!parseTransactionLine(line, transaction) || transaction.from == transaction.to) {
      ++ignoredCount;
      continue;
    }
    transactions.pushBack(transaction);
  }
}

void writeTransaction(std::ostream &output, const Transaction &transaction)
{
  output << transaction.from << ' ' << transaction.to << ' ' << transaction.value << '\n';
}

}
