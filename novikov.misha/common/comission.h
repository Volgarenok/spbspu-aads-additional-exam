#ifndef COMMON_COMMISSION_H_
#define COMMON_COMMISSION_H_

#include <cstddef>
#include <istream>
#include <string>

#include "dynamic_array.h"

namespace novikov {

struct Commission
{
  int account;
  int percent;
  int from;
  int to;
};

struct CommissionInterval
{
  int from;
  int to;
  int percent;
};

bool parseCommissionLine(const std::string &line, Commission &result);

bool readCommissions(std::istream &input, DynamicArray<Commission> &commissions);

DynamicArray<CommissionInterval> mergeCommissionsForAccount(
    const DynamicArray<Commission> &commissions, int signedAccount);

DynamicArray<CommissionInterval> subtractPaidIntervals(
    const DynamicArray<CommissionInterval> &intervals,
    const DynamicArray<CommissionInterval> &paid);

long long computeCommissionAmount(
    const DynamicArray<CommissionInterval> &intervals, int from, int to);

}

#endif
