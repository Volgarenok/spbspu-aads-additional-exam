#include "commission.h"

#include <cctype>
#include <cstdlib>

namespace novikov {

namespace {

bool parseSignedInt(const std::string &line, std::size_t &pos, int &value)
{
  const std::size_t length = line.size();
  while (pos < length && std::isspace(static_cast<unsigned char>(line[pos])) != 0) {
    ++pos;
  }

  bool negative = false;
  if (pos < length && (line[pos] == '-' || line[pos] == '+')) {
    negative = (line[pos] == '-');
    ++pos;
  }
  if (pos >= length || std::isdigit(static_cast<unsigned char>(line[pos])) == 0) {
    return false;
  }

  int result = 0;
  while (pos < length && std::isdigit(static_cast<unsigned char>(line[pos])) != 0) {
    result = result * 10 + (line[pos] - '0');
    ++pos;
  }
  value = negative ? -result : result;
  return true;
}

void sortInts(DynamicArray<int> &values)
{
  for (std::size_t i = 1; i < values.size(); ++i) {
    int key = values[i];
    std::size_t j = i;
    while (j > 0 && values[j - 1] > key) {
      values[j] = values[j - 1];
      --j;
    }
    values[j] = key;
  }
}

}

bool parseCommissionLine(const std::string &line, Commission &result)
{
  std::size_t pos = 0;
  int account = 0;
  int percent = 0;
  int from = 0;
  int to = 0;

  if (!parseSignedInt(line, pos, account) ||
      !parseSignedInt(line, pos, percent) ||
      !parseSignedInt(line, pos, from) ||
      !parseSignedInt(line, pos, to)) {
    return false;
  }

  while (pos < line.size() &&
         std::isspace(static_cast<unsigned char>(line[pos])) != 0) {
    ++pos;
  }
  if (pos != line.size()) {
    return false;
  }
  if (account == 0 || percent < 1 || percent > 100 || from > to) {
    return false;
  }

  result.account = account;
  result.percent = percent;
  result.from = from;
  result.to = to;
  return true;
}

bool readCommissions(std::istream &input, DynamicArray<Commission> &commissions)
{
  std::string line;
  while (std::getline(input, line)) {
    Commission c{0, 0, 0, 0};
    if (!parseCommissionLine(line, c)) {
      return false;
    }
    commissions.pushBack(c);
  }
  return true;
}

DynamicArray<CommissionInterval> mergeCommissionsForAccount(
    const DynamicArray<Commission> &commissions, int signedAccount)
{
  DynamicArray<Commission> filtered;
  for (std::size_t i = 0; i < commissions.size(); ++i) {
    if (commissions[i].account == signedAccount) {
      filtered.pushBack(commissions[i]);
    }
  }

  DynamicArray<CommissionInterval> result;
  if (filtered.size() == 0) {
    return result;
  }

  DynamicArray<int> endpoints;
  for (std::size_t i = 0; i < filtered.size(); ++i) {
    endpoints.pushBack(filtered[i].from);
    endpoints.pushBack(filtered[i].to);
  }
  sortInts(endpoints);

  DynamicArray<int> unique;
  for (std::size_t i = 0; i < endpoints.size(); ++i) {
    if (i == 0 || endpoints[i] != endpoints[i - 1]) {
      unique.pushBack(endpoints[i]);
    }
  }

  for (std::size_t i = 0; i + 1 < unique.size(); ++i) {
    int a = unique[i];
    int b = unique[i + 1];
    int totalPercent = 0;
    for (std::size_t j = 0; j < filtered.size(); ++j) {
      if (filtered[j].from <= a && b <= filtered[j].to) {
        totalPercent += filtered[j].percent;
      }
    }
    if (totalPercent > 0) {
      result.pushBack(CommissionInterval{a, b, totalPercent});
    }
  }

  DynamicArray<CommissionInterval> merged;
  for (std::size_t i = 0; i < result.size(); ++i) {
    if (merged.size() > 0 &&
        merged[merged.size() - 1].to == result[i].from &&
        merged[merged.size() - 1].percent == result[i].percent) {
      merged[merged.size() - 1].to = result[i].to;
    } else {
      merged.pushBack(result[i]);
    }
  }
  return merged;
}

DynamicArray<CommissionInterval> subtractPaidIntervals(
    const DynamicArray<CommissionInterval> &intervals,
    const DynamicArray<CommissionInterval> &paid)
{
  DynamicArray<CommissionInterval> result;
  for (std::size_t i = 0; i < intervals.size(); ++i) {
    int a = intervals[i].from;
    int b = intervals[i].to;
    int pct = intervals[i].percent;

    DynamicArray<int> points;
    points.pushBack(a);
    points.pushBack(b);
    for (std::size_t j = 0; j < paid.size(); ++j) {
      if (paid[j].from > a && paid[j].from < b) {
        points.pushBack(paid[j].from);
      }
      if (paid[j].to > a && paid[j].to < b) {
        points.pushBack(paid[j].to);
      }
    }
    sortInts(points);

    DynamicArray<int> uniq;
    for (std::size_t k = 0; k < points.size(); ++k) {
      if (k == 0 || points[k] != points[k - 1]) {
        uniq.pushBack(points[k]);
      }
    }

    for (std::size_t k = 0; k + 1 < uniq.size(); ++k) {
      int sa = uniq[k];
      int sb = uniq[k + 1];
      int mid = (sa + sb) / 2;
      bool isPaid = false;
      for (std::size_t j = 0; j < paid.size(); ++j) {
        if (paid[j].from <= mid && mid < paid[j].to) {
          isPaid = true;
          break;
        }
      }
      if (!isPaid) {
        result.pushBack(CommissionInterval{sa, sb, pct});
      }
    }
  }
  return result;
}

long long computeCommissionAmount(
    const DynamicArray<CommissionInterval> &intervals, int from, int to)
{
  if (from >= to) {
    return 0;
  }
  long long total = 0;
  for (std::size_t i = 0; i < intervals.size(); ++i) {
    int a = intervals[i].from;
    int b = intervals[i].to;
    int lo = a > from ? a : from;
    int hi = b < to ? b : to;
    if (lo < hi) {
      total += static_cast<long long>(hi - lo) * intervals[i].percent;
    }
  }
  return total / 100;
}

}
