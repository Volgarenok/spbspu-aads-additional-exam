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
    if (line.empty()) {
      continue;
    }

    int raw_from = 0;
    int raw_to = 0;
    int val = 0;
    bool parsed_ok = true;
    std::size_t pos = 0;

    try {
      raw_from = std::stoi(line, &pos);
      std::string rest1 = line.substr(pos);
      raw_to = std::stoi(rest1, &pos);
      std::string rest2 = rest1.substr(pos);
      val = std::stoi(rest2, &pos);
    } catch (...) {
      parsed_ok = false;
    }

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

    Transaction t;
    t.from_ = abs_from;
    t.to_ = abs_to;
    t.value_ = val;

    pushBack(arr, t);
    stats.success_count_++;
  }
}

void printTransactions(std::ostream &out, const DynamicArray<Transaction> &arr)
{
  for (std::size_t i = 0; i < arr.size_; ++i) {
    out << arr.data_[i].from_ << " "
        << arr.data_[i].to_ << " "
        << arr.data_[i].value_ << "\n";
  }
}

}

