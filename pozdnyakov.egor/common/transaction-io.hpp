#ifndef TRANSACTION_IO_HPP
#define TRANSACTION_IO_HPP

#include <ostream>
#include <string>
#include "array.hpp"
#include "transaction.hpp"

namespace pozdnyakov {
  bool parseTransaction(const std::string & line, Transaction & transaction);
  std::ostream & operator<<(std::ostream & out, const Transaction & transaction);
  void writeTransactions(std::ostream & out, const Array< Transaction > & transactions);
}

#endif
