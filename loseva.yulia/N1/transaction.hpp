#ifndef LOSEVA_TRANSACTION_HPP
#define LOSEVA_TRANSACTION_HPP

#include <iostream>
#include "dynamic_array.hpp"

namespace loseva {

struct Transaction {
  int from_;
  int to_;
  int value_;
};

struct ReadStats {
  std::size_t success_count_;
  std::size_t ignored_count_;
};

void readTransactions(std::istream &in, DynamicArray<Transaction> &arr, ReadStats &stats);
void printTransactions(std::ostream &out, const DynamicArray<Transaction> &arr);

}

#endif
