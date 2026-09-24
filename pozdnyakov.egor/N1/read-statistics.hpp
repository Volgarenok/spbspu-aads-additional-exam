#ifndef READ_STATISTICS_HPP
#define READ_STATISTICS_HPP

#include <cstddef>
#include <istream>
#include <array.hpp>
#include <transaction.hpp>

namespace pozdnyakov {
  struct ReadStatistics {
    std::size_t accepted;
    std::size_t ignored;
  };

  ReadStatistics readTransactions(std::istream & in, Array< Transaction > & transactions);
}

#endif
