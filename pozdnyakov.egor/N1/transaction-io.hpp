#ifndef TRANSACTION_IO_HPP
#define TRANSACTION_IO_HPP

#include <cstddef>
#include <istream>
#include <ostream>
#include <string>
#include "array.hpp"
#include "transaction.hpp"

namespace pozdnyakov {
  struct ReadStatistics {
    std::size_t accepted;
    std::size_t ignored;
  };

  bool parseTransaction(const std::string & line, Transaction & transaction);
  ReadStatistics readTransactions(std::istream & in, Array< Transaction > & transactions);
  std::ostream & operator<<(std::ostream & out, const Transaction & transaction);
  void writeTransactions(std::ostream & out, const Array< Transaction > & transactions);
}

#endif
