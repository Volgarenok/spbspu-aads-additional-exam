#include "transaction.hpp"
#include <cmath>
#include <string>

namespace loseva {

void readTransactions(std::istream &in, DynamicArray<Transaction> &arr, ReadStats &stats)
{
  stats.success_count_ = 0;
  stats.ignored_count_ = 0;
  std::string line;

  while (std::getline(in, line)) {
    int raw_from = 0;
    int raw_to = 0;
    int val = 0;
    bool parsed_ok = true;
    std::size_t pos = 0;

    if (!parsed_ok) {
      stats.ignored_count_++;
      continue;
    }

    const int abs_from = std::abs(raw_from);
    const int abs_to = std::abs(raw_to);

    if (abs_from == abs_to) {
      stats.ignored_count_++;
      continue;
    }

    pushBack(arr, t);
    stats.success_count_++;
  }
}

}
