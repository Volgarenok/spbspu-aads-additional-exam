#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include "array.hpp"

namespace smirnova {
  struct Transaction {
    int from, to;
    int value;
  };

  inline long abs_long(const long v) {
    return v < 0 ? -v : v;
  }

  inline bool parse_int(const char*& p, long& result) {
    char* end = nullptr;
    long val = std::strtol(p, &end, 10);
    if (end == p) {
      return false;
    }
    p = end;
    result = val;
    return true;
  }

  inline void skip_ws(const char*& p) {
    while (*p == ' ' || *p == '\t' || *p == '\n' ||
           *p == '\r' || *p == '\f' || *p == '\v') {
      ++p;
    }
  }

  inline bool parse_transaction(const std::string& line, Transaction& t) {
    const char* p = line.c_str();
    long from = 0;
    long to = 0;
    long value = 0;

    if (!parse_int(p, from)) {
      return false;
    }
    if (!parse_int(p, to)) {
      return false;
    }
    if (!parse_int(p, value)) {
      return false;
    }

    skip_ws(p);
    if (*p != '\0') {
      return false;
    }

    t.from = static_cast<int>(abs_long(from));
    t.to = static_cast<int>(abs_long(to));
    t.value = static_cast<int>(value);
    return true;
  }

  inline void read_transactions(std::istream& in,
                                Array<Transaction>& valid,
                                std::size_t& ignored_count) {
    ignored_count = 0;
    std::string line;

    while (std::getline(in, line)) {
      Transaction t;
      if (parse_transaction(line, t)) {
        if (t.from != t.to) {
          array_push(valid, t);
        } else {
          ++ignored_count;
        }
      } else {
        ++ignored_count;
      }
    }
  }

  inline void write_transactions(std::ostream& out,
                                 const Array<Transaction>& arr) {
    for (std::size_t i = 0; i < arr.size; ++i) {
      out << arr.data[i].from << ' '
          << arr.data[i].to << ' '
          << arr.data[i].value << '\n';
    }
  }
}

#endif
