#ifndef LOSEVA_COMMISSION_HPP
#define LOSEVA_COMMISSION_HPP

#include <iostream>
#include "dynamic_array.hpp"

namespace loseva {

struct Commission {
  int account_;
  int percent_;
  int start_;
  int end_;
  bool is_incoming_;
};

struct EffectiveInterval {
  int start_;
  int end_;
  int total_percent_;
};

bool readCommissions(std::istream &in, DynamicArray<Commission> &commissions);
void calculateEffectiveIntervals(
  const DynamicArray<Commission> &commissions,
  int account_id,
  bool is_incoming,
  DynamicArray<EffectiveInterval> &result
);

}

#endif

