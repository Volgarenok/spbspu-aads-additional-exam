#include "read-statistics.hpp"
#include <istream>
#include <string>
#include <array.hpp>
#include <parsing.hpp>
#include <transaction-io.hpp>
#include <transaction.hpp>

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
