#include <transaction.h>

#include <cctype>
#include <cstdlib>

namespace novikov {

namespace {

constexpr int k_decimal_base = 10;
constexpr int k_zero_char = '0';

bool isDigitChar(char c) {
  return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

bool isSpaceChar(char c) {
  return std::isspace(static_cast<unsigned char>(c)) != 0;
}

bool parseSignedInt(const std::string &line, std::size_t &pos, int &value) {
  const std::size_t length = line.size();
  while (pos < length && isSpaceChar(line[pos])) {
    ++pos;
  }

  bool negative = false;
  if (pos < length && (line[pos] == '-' || line[pos] == '+')) {
    negative = (line[pos] == '-');
    ++pos;
  }

  if (pos >= length || !isDigitChar(line[pos])) {
    return false;
  }

  int result = 0;
  while (pos < length && isDigitChar(line[pos])) {
    result = result * k_decimal_base + (line[pos] - k_zero_char);
    ++pos;
  }
  value = negative ? -result : result;
  return true;
}

}

bool parseTransactionLine(const std::string &line, Transaction &result) {
  std::size_t pos = 0;
  int from = 0;
  int to = 0;
  int value = 0;

  if (!parseSignedInt(line, pos, from) || !parseSignedInt(line, pos, to) ||
      !parseSignedInt(line, pos, value)) {
    return false;
  }

  while (pos < line.size() && isSpaceChar(line[pos])) {
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

void readTransactions(std::istream &input,
                      DynamicArray<Transaction> &transactions,
                      std::size_t &ignored_count) {
  std::string line;
  while (std::getline(input, line)) {
    bool has_content = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
      if (!isSpaceChar(line[i])) {
        has_content = true;
        break;
      }
    }
    if (!has_content) {
      continue;
    }

    Transaction transaction{0, 0, 0};
    if (!parseTransactionLine(line, transaction) ||
        transaction.from == transaction.to) {
      ++ignored_count;
      continue;
    }
    transactions.pushBack(transaction);
  }
}

void writeTransaction(std::ostream &output, const Transaction &transaction) {
  output << transaction.from << ' ' << transaction.to << ' '
         << transaction.value << '\n';
}

}
