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

}

#endif
