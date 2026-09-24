#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <cstddef>
#include <istream>
#include <ostream>
#include "dynamic_array.hpp"

namespace loseva {

struct Transaction {
  int from;
  int to;
  int value;
};

struct ReadStats {
  std::size_t success_count;
  std::size_t ignored_count;
};

void readTransactions(std::istream &in, DynamicArray<Transaction> &arr, ReadStats &stats);
void printTransactions(std::ostream &out, const DynamicArray<Transaction> &arr);

}

#endif

