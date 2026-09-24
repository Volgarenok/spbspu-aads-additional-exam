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
      long res = 0;
      if ( v < 0 ) {
        res = -v;
      } else {
        res = v;
      }
      return res;
  }

  inline bool parse_int(const char*& p, long& result) {
    char* end;
    long val = std::strtol(p, &end, 10);
    if (end == p) {
      return false;
    }
    p = end;
    result = val;
    return true;
  }

  inline bool parse_transaction(const std::string& line, Transaction& t) {
    const char* p = line.c_str();
    long from, to, value;
    bool res = false;
    if (parse_int(p, from) && parse_int(p, to) && parse_int(p, value)) {
      res = true;
      t.from = static_cast< int >(abs_long(from));
      t.to = static_cast< int >(abs_long(to));
      t.value = static_cast< int >(value);
    }

    return res;
  }

  inline void read_transactions(std::istream& in, Array<Transaction>& valid,std::size_t& ignored_count) {
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

  inline void write_transactions(std::ostream& out, const Array< Transaction >& arr) {
    for (std::size_t i = 0; i < arr.size; ++i) {
      out << arr.data[i].from << ' ' << arr.data[i].to << ' ' << arr.data[i].value << '\n';
    }
  }
}

#endif
