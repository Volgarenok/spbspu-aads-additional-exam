#include "transaction-io.hpp"
#include <cstddef>
#include <ostream>
#include <string>
#include "array.hpp"
#include "parsing.hpp"
#include "transaction.hpp"

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
  if (!isLineEnd(line, position)) {
    return false;
  }
  transaction = result;
  return true;
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
