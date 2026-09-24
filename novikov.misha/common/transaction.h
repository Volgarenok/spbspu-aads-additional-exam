#ifndef COMMON_TRANSACTION_H_
#define COMMON_TRANSACTION_H_

#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

#include "dynamic_array.h"

namespace novikov {

struct Transaction
{
  int from;
  int to;
  int value;
};

bool parseTransactionLine(const std::string &line, Transaction &result);

void readTransactions(std::istream &input, DynamicArray<Transaction> &transactions, std::size_t &ignoredCount);

void writeTransaction(std::ostream &output, const Transaction &transaction);

}

#endif
