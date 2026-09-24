#include "transaction.hpp"
#include <cmath>
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>
#include "dynamic_array.hpp"

namespace loseva {

void readTransactions(std::istream &in, DynamicArray<Transaction> &arr, ReadStats &stats)
{
  stats.success_count = 0;
  stats.ignored_count = 0;
  std::string line;

  while (std::getline(in, line)) {
    int raw_from = 0;
    int raw_to = 0;
    int val = 0;
    bool parsed_ok = true;
    std::size_t pos = 0;

    try {
      raw_from = std::stoi(line, &pos);
      const std::string rest1 = line.substr(pos);
      raw_to = std::stoi(rest1, &pos);
      const std::string rest2 = rest1.substr(pos);
      val = std::stoi(rest2, &pos);
    } catch (...) {
      parsed_ok = false;
    }

    if (!parsed_ok) {
      stats.ignored_count++;
      continue;
    }

    const int abs_from = std::abs(raw_from);
    const int abs_to = std::abs(raw_to);

    if (abs_from == abs_to) {
      stats.ignored_count++;
      continue;
    }

    Transaction t;
    t.from = abs_from;
    t.to = abs_to;
    t.value = val;

    pushBack(arr, t);
    stats.success_count++;
  }
}

void printTransactions(std::ostream &out, const DynamicArray<Transaction> &arr)
{
  for (std::size_t i = 0; i < arr.size; ++i) {
    out << arr.data[i].from << " "
        << arr.data[i].to << " "
        << arr.data[i].value << "\n";
  }
}

}

